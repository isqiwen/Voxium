import os
import argparse

IGNORE_FOLDER = ["build", "bin", "dist", ".git", ".venv", ".vscode"]

def should_ignore(root):
    return any(ignored in root.split(os.sep) for ignored in IGNORE_FOLDER)

def create_gitkeep(directory="."):
    if should_ignore(directory):
        return

    for root, dirs, files in os.walk(directory):
        if should_ignore(root):
            continue

        dirs[:] = [d for d in dirs if d not in IGNORE_FOLDER]

        for d in dirs:
            dir_path = os.path.join(root, d)

            if not os.listdir(dir_path):
                open(os.path.join(dir_path, ".gitkeep"), 'w').close()
                print(f"Created: {os.path.join(dir_path, '.gitkeep')}")

def _create_parser(parent_parser = None):
    description = "create .gitkeep in empty folder."
    if parent_parser is None:
        parser = argparse.ArgumentParser(description=description)
    else:
        parser = parent_parser.add_parser("gitkeep", description=description, help=description)

    parser.add_argument(
        "path",
        type=str,
        help="The root directory to create .gitkeep."
    )

    return parser

def register_subcommand(parent_parse):
    """
    Add 'gitkeep' subcommand to the parent parser.
    """
    parser = _create_parser(parent_parse)

    parser = parser.set_defaults(func=lambda args: create_gitkeep(args.path))

def main():
    """
    Entry point for the script. Parses command-line arguments and calls `create_gitkeep`.
    """
    parser = _create_parser()

    args = parser.parse_args()

    if args.path:
        create_gitkeep(args.path, args.depth)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
