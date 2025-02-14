import sys
from pathlib import Path

tools_package_location = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(tools_package_location))

from Tools.BuildSystem.ProjectConfig import ProjectConfig
from Tools.Utilities.Logger import Logger

from Tools.BuildSystem.ConanHelper import install_conan, configure_conan
from Tools.BuildSystem.VenvHelper import check_pipenv


def conan_setup():
    Logger.Info("Conan setup begin!")

    # Check for required tools
    check_pipenv()

    # Install Conan
    install_conan()

    # Configure Conan
    configure_conan()

    Logger.Info("Conan setup end!")


if __name__ == "__main__":
    ProjectConfig.initialize(tools_package_location / "res" / "config" / "project_config.json")

    conan_setup()
