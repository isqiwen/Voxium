from pathlib import Path
import json

class ConfigLoader:
    def __init__(self, config_path):
        self.config_path = Path(config_path)
        self.config = self._load_config()

    def _load_config(self):
        if not self.config_path.exists():
            raise FileNotFoundError(f"Configuration file not found: {self.config_path}")

        try:
            with open(self.config_path, "r") as f:
                return json.load(f)
        except json.JSONDecodeError as e:
            raise ValueError(f"Invalid JSON in configuration file: {e}")

    def get(self, key_path, default=None):
        """
        Access nested configuration using a dotted key path (e.g., "platforms.linux.build_dir").
        """
        keys = key_path.split(".")
        value = self.config
        for key in keys:
            if key in value:
                value = value[key]
            else:
                return default
        return value
