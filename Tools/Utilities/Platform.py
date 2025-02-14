import sys


class Platform:
    WINDOWS = "windows"
    LINUX = "linux"
    MACOS = "macos"
    UNKNOWN = "unknown"

    @staticmethod
    def detect():
        if sys.platform.startswith("win32"):
            return Platform.WINDOWS
        elif sys.platform.startswith("linux"):
            return Platform.LINUX
        elif sys.platform.startswith("darwin"):
            return Platform.MACOS
        else:
            raise Exception(f"Unsupported platform: {sys.platform}")

    @staticmethod
    def is_unix_like():
        """
        Check if the platform is Unix-like (Linux or macOS).
        """
        return Platform.detect() in {Platform.LINUX, Platform.MACOS}

    @staticmethod
    def is_windows():
        """
        Check if the platform is Windows.
        """
        return Platform.detect() == Platform.WINDOWS

    @staticmethod
    def is_linux():
        """
        Check if the platform is Linux.
        """
        return Platform.detect() == Platform.LINUX

    @staticmethod
    def is_macos():
        """
        Check if the platform is Macos.
        """
        return Platform.detect() == Platform.MACOS
