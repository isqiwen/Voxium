import os
import sys
import asyncio
from pathlib import Path

tools_package_location = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(tools_package_location))

from Tools.Utilities.Logger import Logger


class DecodeFailed(Exception):
    pass


class Decoder:

    static_supported_encodings = ('utf-8-sig', 'utf-8', 'gbk', 'big5')

    @classmethod
    def Decode(cls, bytes):
        for encoding in cls.static_supported_encodings:
            try:
                return bytes.decode(encoding)
            except UnicodeDecodeError as err:
                pass

        Logger.Error(f"Decoder: {bytes}")
        Logger.Error(f"Decoder: Decoding failed, supported coding : {cls.static_supported_encodings}")
        raise DecodeFailed()


class StringBuffer:

    def __init__(self):
        self.myData = ''

    @property
    def data(self):
        return self.myData

    def Append(self, data):
        self.myData += data


class SubprocessLogReaderAsync:
    """
    Reads lines asynchronously from a subprocess stream, decodes them, and logs or buffers the output.
    """

    def __init__(self, stream, string_buffer, log_method, log_each_line=True):
        """
        Initialize the log reader.

        Args:
            stream (asyncio.StreamReader): The stream to read from.
            string_buffer (StringBuffer): A buffer to accumulate all lines read.
            log_method (callable): A logging method (e.g., logging.info or logging.error).
            log_each_line (bool): Whether to log each line or only at the end.
        """
        self.stream = stream
        self.string_buffer = string_buffer
        self.line_logger = log_method if log_each_line else lambda _: None
        self.passage_logger = log_method if not log_each_line else lambda _: None

    async def __call__(self):
        """
        Start reading lines asynchronously from the stream.

        This function appends decoded lines to the buffer and logs them based on the settings.
        """
        async for line_bytes in self.stream:
            line = Decoder.Decode(line_bytes).rstrip()
            self.string_buffer.Append(line + '\n')
            self.line_logger(line)
        # Log the complete passage if `log_each_line` is False
        self.passage_logger(self.string_buffer.data)


async def run_command_async(command, *, check=False, env=None, log_each_line=True, log_stdout=Logger.Info, log_stderr=Logger.Error, **kwargs):
    """
    Run a shell command asynchronously.

    Args:
        command (str or list): Command to run. String for Windows, list for Linux/Unix.
        check (bool): Whether to raise an exception if the process exits with a non-zero code.
        env (dict or None): A dictionary of environment variables to pass to the subprocess.
            - If `None` (default), the subprocess inherits the current process's environment variables.
        log_stdout (callable): Optional function to log stdout lines (e.g., print or a logger).
        log_stderr (callable): Optional function to log stderr lines (e.g., print or a logger).
        **kwargs: Additional keyword arguments for create_subprocess_exec.

    Returns:
        tuple: (exit_code, stdout, stderr) where:
            - exit_code: Process exit code.
            - stdout: Captured stdout as a single string.
            - stderr: Captured stderr as a single string.

    Raises:
        ShellCommandError: If `check` is True and the process exits with a non-zero code.
    """
    if isinstance(command, str):
        command = command.split()
    elif not isinstance(command, list):
        raise ValueError("Command must be a string or a list.")

    Logger.Info(f'Shell: executing shell command : {command}')

    # Merge environment variables
    final_env = os.environ.copy()
    if env:
        final_env.update(env)

    try:
        process = await asyncio.create_subprocess_exec(
            *command,
            env=final_env,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE,
            **kwargs
        )
    except Exception as e:
        Logger.Error(f"Failed to start process: {e}")
        raise

    stdout_buffer = StringBuffer()
    stderr_buffer = StringBuffer()

    stdout_reader = SubprocessLogReaderAsync(
        process.stdout,
        stdout_buffer,
        log_method=log_stdout,
        log_each_line=log_each_line
    )

    stderr_reader = SubprocessLogReaderAsync(
        process.stderr,
        stderr_buffer,
        log_method=log_stderr,
        log_each_line=log_each_line
    )

    try:
        await asyncio.gather(stdout_reader(), stderr_reader())
        return_code = await process.wait()
        stdout, stderr = stdout_buffer.data.rstrip(), stderr_buffer.data.rstrip()

        if not log_each_line:
            len(stdout) > 0 and log_stdout(stdout)
            len(stderr) > 0 and log_stdout(stderr)

        if check and return_code != 0:
            raise RuntimeError(f"Command {command} failed with return code {return_code}")
        else:
            return process.returncode == 0, stdout, stderr
    except Exception as e:
        Logger.Error(f"Error while running command: {e}")
        raise


def run_command(*args, **kwargs):
    return asyncio.run(run_command_async(*args, **kwargs))


if __name__ == "__main__":
    command = "cmake --version"
    try:
        return_code, stdout, stderr = run_command(command, check=True)
    except Exception as e:
        Logger.Error(f"Command failed: {e}")
