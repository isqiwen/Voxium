import os
import sys
import argparse
from pathlib import Path

tools_package_location = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(tools_package_location))

from Tools.BuildSystem.VenvHelper import get_executable_path
from Tools.Utilities.Logger import Logger
from Tools.Utilities.ShellUtils import run_command

def run_clang_format_command(command_args, *, check=True):
    """
    Args:
        command_args (list): List of arguments for the clang-format command.
    """
    if isinstance(command_args, str):
        # Split string into list for processing
        command_parts = command_args.split()
    elif isinstance(command_args, list):
        # Use the list directly
        command_parts = command_args[:]
    else:
        raise TypeError("Command args must be a string or a list.")

    clang_format_path = get_executable_path("clang-format")

    if clang_format_path == None:
        raise FileNotFoundError("clang-format was not found.")

    if Path(command_parts[0]).stem.lower() == "clang-format":
        command_parts[0] = str(clang_format_path)
    else:
        command_parts.insert(0, str(clang_format_path))

    return run_command(command_parts, check=check)

def format_code(path, style="file", recursive=False):
    """
    Format code files using clang-format.

    Args:
        path (str): The file or directory to format.
        style (str): The clang-format style to use (default: "Google").
        recursive (bool): Whether to recursively format files in a directory.
    """
    if not os.path.exists(path):
        Logger.Error(f"Path not found: {path}")
        return

    if os.path.isfile(path):
        _format_file(path, style)
    elif os.path.isdir(path):
        if recursive:
            for root, _, files in os.walk(path):
                for file in files:
                    if file.endswith(('.cpp', '.h', '.c', '.hpp', '.cc')):
                        _format_file(os.path.join(root, file), style)
        else:
            for file in os.listdir(path):
                full_path = os.path.join(path, file)
                if os.path.isfile(full_path) and file.endswith(('.cpp', '.h', '.c', '.hpp', '.cc')):
                    _format_file(full_path, style)
    else:
        Logger.Error(f"Invalid path: {path}")

def _format_file(file_path, style):
    """Helper function to format a single file using clang-format."""
    try:
        Logger.Info(f"Formatting: {file_path}")
        run_clang_format_command(f"clang-format -i {file_path} --style={style}")
    except Exception as e:
        Logger.Error(f"Failed to format {file_path}: {e}")

def _create_parser(parent_parser=None):
    description = "Formats code files using clang-format. Supports recursive formatting."
    if parent_parser is None:
        parser = argparse.ArgumentParser(description=description)
    else:
        parser = parent_parser.add_parser("format", description=description, help=description)

    parser.add_argument(
        "path",
        type=Path,
        help="The file or directory to format."
    )
    parser.add_argument(
        "-s", "--style",
        type=str,
        default="file",
        help="The clang-format style to use (default: 'file')."
    )
    parser.add_argument(
        "-r", "--recursive",
        action="store_true",
        help="Recursively format files in a directory."
    )

    return parser

def register_subcommand(parent_parser):
    """
    Add 'format' subcommand to the parent parser.
    """
    parser = _create_parser(parent_parser)

    parser.set_defaults(func=lambda args: format_code(os.path.normpath(args.path), args.style, args.recursive))

def main():
    """
    Entry point for the script. Parses command-line arguments and calls `format_code`.
    """
    parser = _create_parser()

    args = parser.parse_args()

    if args.path:
        format_code(args.path, args.style, args.recursive)
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
