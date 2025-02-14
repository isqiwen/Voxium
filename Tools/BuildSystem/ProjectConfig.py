import sys
from pathlib import Path

tools_package_location = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(tools_package_location))

from Tools.Utilities.Logger import Logger
from Tools.Utilities.Platform import Platform
from Tools.BuildSystem.ConfigLoader import ConfigLoader


class ProjectConfig:
    """
    Singleton-style class for managing global project configuration.
    """
    _initialized = False

    PROJECT_NAME = None
    PLATFORM     = Platform.detect()
    PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
    BUILD_DIR = None
    SOURCE_DIR = None
    DIST_DIR = None
    COMPILER_CPPSTD = 17
    BUILD_TYPE = None
    CMAKE_GENERATOR = None

    CONAN_USER_HOME = None
    CONAN_PROFILE   = "default"

    @classmethod
    def initialize(cls, build_config_file=None):
        """
        Initialize the configuration from a JSON file.

        Args:
            build_config_file (str): Path to the build configuration file.

        Raises:
            FileNotFoundError: If the configuration file does not exist.
            ValueError: If the configuration file contains invalid JSON.
        """
        if cls._initialized:
            return

        if build_config_file is None:
            build_config_file = cls.PROJECT_ROOT / "config" / "project_config.json"

        if not build_config_file.exists() or not build_config_file.is_file():
            raise FileNotFoundError(f"Configuration file not found: {build_config_file}")

        cls._load_build_config_json(build_config_file)

        cls._initialized = True

    @classmethod
    def _load_build_config_json(cls, build_config_file):
        """
        Load build configuration from a JSON file.
        """
        config_data = ConfigLoader(build_config_file)

        # Assign configuration values
        cls.PROJECT_NAME = config_data.get("project_name")
        cls.BUILD_DIR = config_data.get("build_dir")
        cls.SOURCE_DIR = config_data.get("source_dir")
        cls.DIST_DIR = config_data.get("dist_dir")
        cls.COMPILER_CPPSTD = config_data.get("compiler.cppstd")
        cls.BUILD_TYPE = config_data.get("build_type")
        cls.CMAKE_GENERATOR = config_data.get("cmake_generator")

        conan_user_home_template = config_data.get(f"conan.user_home.{cls.PLATFORM}")
        conan_user_home = conan_user_home_template.format(project_name=cls.PROJECT_NAME)
        cls.CONAN_USER_HOME = Path(cls.PROJECT_ROOT.drive) / conan_user_home if Platform.is_windows() else Path(conan_user_home)

        cls.CONAN_PROFILE = config_data.get("conan.profile")

    @classmethod
    def reload(cls, *, build_config_file):
        """
        Reload the configuration from a file, even if already initialized.
        """
        cls._initialized = False
        cls.initialize(build_config_file)

    @classmethod
    def summary(cls):
        """
        Summarize the current configuration values.

        Returns:
            str: A formatted string containing the summary of configuration values.
        """
        if not cls._initialized:
            return "Configuration has not been initialized."

        summary_lines = [
            "Project Configuration Summary:",
            f"  BUILD_DIR        : {cls.BUILD_DIR}",
            f"  SOURCE_DIR       : {cls.SOURCE_DIR}",
            f"  DIST_DIR         : {cls.DIST_DIR}",
            f"  CONAN_PROFILE    : {cls.CONAN_PROFILE}",
            f"  COMPILER_STD     : {cls.COMPILER_CPPSTD}",
            f"  BUILD_TYPE       : {cls.BUILD_TYPE}",
            f"  CMAKE_GENERATOR  : {cls.CMAKE_GENERATOR}",
        ]

        Logger.Info("\n".join(summary_lines))


if __name__ == "__main__":
    ProjectConfig.initialize(tools_package_location / "res" / "config" / "project_config.json")
    ProjectConfig.summary()
