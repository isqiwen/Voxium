import logging
import logging.config
import json
from pathlib import Path


class LoggerSingleton:
    """
    A singleton logger class that provides simple static methods for logging.
    """
    _instance = None

    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            cls._instance = super(LoggerSingleton, cls).__new__(cls, *args, **kwargs)
            cls._instance._initialized = False
        return cls._instance

    def __init__(self, config_file=None, log_level=logging.INFO):
        if not self._initialized:
            self._initialized = True
            self.logger = None
            if config_file:
                self._setup_from_config_file(config_file)
            else:
                self._setup_default(log_level)

    def _setup_from_config_file(self, config_file):
        """
        Set up logging from a JSON configuration file.
        """
        config_path = Path(config_file)
        if not config_path.is_file():
            raise FileNotFoundError(f"Logging configuration file not found: {config_file}")

        with open(config_path, "r") as f:
            config = json.load(f)
        logging.config.dictConfig(config)
        self.logger = logging.getLogger()

    def _setup_default(self, log_level):
        """
        Set up default logging configuration.
        """
        log_format = "%(asctime)s [%(levelname)s] %(message)s"
        logging.basicConfig(level=log_level, format=log_format)
        self.logger = logging.getLogger()

    @staticmethod
    def Debug(message, *args, **kwargs):
        LoggerSingleton().logger.debug(message, *args, **kwargs)

    @staticmethod
    def Info(message, *args, **kwargs):
        LoggerSingleton().logger.info(message, *args, **kwargs)

    @staticmethod
    def Warning(message, *args, **kwargs):
        LoggerSingleton().logger.warning(message, *args, **kwargs)

    @staticmethod
    def Error(message, *args, **kwargs):
        LoggerSingleton().logger.error(message, *args, **kwargs)

    @staticmethod
    def Critical(message, *args, **kwargs):
        LoggerSingleton().logger.critical(message, *args, **kwargs)

    @staticmethod
    def set_level(level):
        LoggerSingleton().logger.set_level(level)


# Global logger instance
Logger = LoggerSingleton
