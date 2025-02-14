import os
import sys
from pathlib import Path

from Tools.Utilities.ShellUtils import run_command
from Tools.Utilities.Logger import Logger
from Tools.Utilities.FileUtils import clean


class EnvironmentSetupError(Exception):
    """Custom exception for environment setup errors."""
    pass

def get_executable_path(executable_name):
    success, venv_path = get_pipenv_venv()
    venv_path = Path(venv_path)

    if not success:
        Logger.Error(f"Cannot detect pipenv environment in {os.get_cwd()}")
        return None

    executable_name = Path(executable_name).stem

    candidates = [
        venv_path / "bin" / executable_name, # Unix-like systems
        venv_path / "Scripts" / (executable_name + ".exe") # Windows
    ]

    executable_path = None

    for p in candidates:
        if p.exists() and p.is_file():
            executable_path = p
            break

    if executable_path == None:
        Logger.Error(f"No {executable_name} interpreter found in virtual environment: {venv_path}")
        return None

    Logger.Info(f"Detect {executable_name} in virtual environment: {venv_path}")

    return executable_path

def get_python_info():
    """
    Detect Python version and ensure it meets the requirements (>= 3.7).
    """
    _, version_output, _ = run_command(["python", "--version"])
    Logger.Info(f"Detect Python version: {version_output}")

    version_parts = version_output.split()[1].split(".")
    major, minor = int(version_parts[0]), int(version_parts[1])

    if major < 3 or (major == 3 and minor < 7):
        raise EnvironmentSetupError(f"Python 3.7 or higher is required!")

    return f"{major}.{minor}", sys.executable

def get_virtualenv_python_info(venv_path):
    """
    Locate the Python interpreter within the given virtual environment path.

    Args:
        venv_path (str or Path): Path to the virtual environment root.

    Returns:
        str: Full path to the Python interpreter in the virtual environment.
        None: If no valid Python interpreter is found.
    """

    python_path = get_executable_path("python")

    if python_path == None:
        raise FileNotFoundError("python was not found.")

    _, version_output, _ = run_command([str(python_path), "--version"])

    Logger.Info(f"Detect Python version: {version_output}")

    return python_path, version_output

def get_pipenv_venv():
    """
    Check if the virtual environment already exists.
    """
    success, venv_path, _ = run_command(["pipenv", "--venv"], check=False)
    return success, venv_path

def delete_virtualenv(target_root):
    """
    Delete the existing virtual environment.
    """
    Logger.Info("Deleting existing virtual environment...")
    run_command(["pipenv", "--rm"])

    venv_path = Path(target_root) / ".venv"
    if venv_path.exists():
        Logger.Info(f"Force deleting virtual environment directory: {venv_path}")
        clean(venv_path)
    else:
        Logger.Info("No .venv directory found, skipping manual deletion.")

def check_pipenv():
    """
    Check if Pipenv is installed.
    """
    success, venv_path, _ = run_command(["pipenv", "--version"], check=False)

    if not success:
        raise EnvironmentSetupError("Pipenv doesn't exist!")

    return success, venv_path

def activate_pipenv_env():
    """
    Ensure Pipenv environment is active and install required packages.
    """
    Logger.Info("Activating pipenv virtual environment...")
    # Check if a Pipenv virtual environment exists
    success, venv_path, _ = run_command(["pipenv", "--venv"], check=False)

    if success:
        Logger.Info(f"Pipenv virtual environment detected: {venv_path}")
    else:
        raise EnvironmentSetupError("No Pipenv environment found!")

    # Activate the virtual environment
    env = os.environ.copy()
    env["PIPENV_VENV_IN_PROJECT"] = "TRUE"  # Use in-project virtual environment

    return env

def create_virtualenv(python_version, pypi_source):
    """
    Initialize a pipenv virtual environment in the target root directory.
    """

    # Set environment variables
    env = os.environ.copy()
    env["PIPENV_VENV_IN_PROJECT"] = "TRUE"
    env["PIPENV_MAX_DEPTH"] = "10"

    Logger.Info("Upgrading pip...")
    run_command(["python", "-m", "pip", "install", "--upgrade", "pip", "-i", pypi_source], env=env)

    Logger.Info("Installing pipenv...")
    run_command(["python", "-m", "pip", "install", "pipenv", "-i", pypi_source], env=env)

    Logger.Info("Initializing virtualenv...")
    run_command(["pipenv", "--python", python_version], env=env)

    Logger.Info("Upgrading pip in virtualenv...")
    run_command(["pipenv", "install", "-i", pypi_source], env=env)

    Logger.Info("Installing cmake in virtualenv...")
    run_command(["pipenv", "install", "cmake", "-i", pypi_source], env=env)

    Logger.Info("Installing clang-format in virtualenv...")
    run_command(["pipenv", "install", "clang-format", "-i", pypi_source], env=env)

    Logger.Info("Installing clang-tidy in virtualenv...")
    run_command(["pipenv", "install", "clang-tidy", "-i", pypi_source], env=env)

def run_pipenv_python_command(command : str | list, *, check=True):
    env = os.environ.copy()
    env["PIPENV_VENV_IN_PROJECT"] = "TRUE"
    env["PIPENV_MAX_DEPTH"] = "10"

    if isinstance(command, str):
        # Split string into list for processing
        command_parts = command.split()
    elif isinstance(command, list):
        # Use the list directly
        command_parts = command[:]
    else:
        raise TypeError("Command must be a string or a list.")

    python_path = get_executable_path("python")

    if python_path == None:
        raise FileNotFoundError("python was not found.")

    # Replace 'python' with the specified python_path if it's the first element
    if Path(command_parts[0]).stem == "python" or Path(command_parts[0]).stem == "python3":
        command_parts[0] = python_path
    else:
        command_parts.insert(0, "-m")
        command_parts.insert(0, str(python_path))

    Logger.Info(f"Run: {' '.join(command_parts)}")

    return run_command(command_parts, check=check, env=env)
