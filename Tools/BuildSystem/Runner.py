import sys
from pathlib import Path
import argparse

tools_package_location = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(tools_package_location))

from Tools.BuildSystem.ProjectConfig import ProjectConfig
from Tools.BuildSystem.Builder import Builder


def run(config):
    builder = Builder()
    builder.setup_and_run(config)

def _create_parser(parent_parser = None):
    description = ProjectConfig.PROJECT_NAME
    if parent_parser is None:
        parser = argparse.ArgumentParser(description=description)
    else:
        parser = parent_parser.add_parser(ProjectConfig.PROJECT_NAME, description=description, help=description)

    workflow = parser.add_argument_group('workflow')

    workflow.add_argument(
        "--install-dp",
        dest = 'install_dp',
        help = f'Install {ProjectConfig.PROJECT_NAME} dependencies',
        default = False,
        action = 'store_true'
    )

    workflow.add_argument(
        "--clean",
        dest = 'clean',
        help = f'Clean {ProjectConfig.PROJECT_NAME}',
        default = False,
        action = 'store_true'
    )

    workflow.add_argument(
        "--build",
        dest = 'build',
        help = f'Build {ProjectConfig.PROJECT_NAME}',
        default = False,
        action = 'store_true'
    )

    workflow.add_argument(
        "--pack",
        dest = 'pack',
        help = f'Pack {ProjectConfig.PROJECT_NAME}',
        default = False,
        action = 'store_true'
    )

    workflow.add_argument(
        "--test",
        dest = 'test',
        help = f'Test {ProjectConfig.PROJECT_NAME}',
        default = False,
        action = 'store_true'
    )

    flags = parser.add_argument_group('flags')

    flags.add_argument(
        "--debug",
        dest = 'debug',
        help = 'Build Debug Version',
        default = False,
        action = 'store_true'
    )

    flags.add_argument(
        "--verbose",
        dest = 'verbose',
        help = "Build with verbose log",
        default = False,
        action = 'store_true'
    )

    return parser

def register_subcommand(parent_parse):
    """
    Add 'build-eurora' subcommand to the parent parser.
    """
    parser = _create_parser(parent_parse)

    parser = parser.set_defaults(func=run)

def main():
    """
    Entry point for the script. Parses command-line arguments and calls `build-eurora`.
    """
    parser = _create_parser()

    args = parser.parse_args()

    if args.path:
        run(args)
    else:
        parser.print_help()


if __name__ == "__main__":
    ProjectConfig.initialize(tools_package_location / "res" / "config" / "project_config.json")

    main()
