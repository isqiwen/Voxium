import os
import argparse


def print_tree(path, depth=1, indent=""):
    """
    Recursively prints the directory structure up to a specified depth.

    Args:
        path (str): The root directory path to start printing.
        depth (int): Maximum depth to recurse into subdirectories.
        indent (str): Indentation string for each level.
    """
    if depth < 0:
        return
    try:
        items = os.listdir(path)
    except PermissionError:
        print(f"{indent}[Permission Denied: {path}]")
        return
    except FileNotFoundError:
        print(f"{indent}[Path Not Found: {path}]")
        return

    for item in items:
        full_path = os.path.join(path, item)
        print(f"{indent}{item}")
        if os.path.isdir(full_path):
            print_tree(full_path, depth - 1, indent + "    ")

def _create_parser(parent_parser = None):
    description = "Prints the directory tree structure up to a specified depth."
    if parent_parser is None:
        parser = argparse.ArgumentParser(description=description)
    else:
        parser = parent_parser.add_parser("tree", description=description, help=description)

    parser.add_argument(
        "path",
        type=str,
        help="The root directory to start printing."
    )
    parser.add_argument(
        "-d", "--depth",
        type=int,
        default=1,
        help="The maximum depth of recursion (default: 1)."
    )

    return parser

def register_subcommand(parent_parse):
    """
    Add 'print-tree' subcommand to the parent parser.
    """
    parser = _create_parser(parent_parse)

    parser = parser.set_defaults(func=lambda args: print_tree(args.path, args.depth))

def main():
    """
    Entry point for the script. Parses command-line arguments and calls `print_tree`.
    """
    parser = _create_parser()

    args = parser.parse_args()

    if args.path:
        print_tree(args.path, args.depth)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
