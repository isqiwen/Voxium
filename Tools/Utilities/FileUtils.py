import shutil
from pathlib import Path

from Tools.Utilities.Logger import Logger


def clean(target, dry_run=False):
    """
    A general-purpose function to clean files or directories.

    Args:
        target (str | Path): The path to the file or directory to clean.
        dry_run (bool): If True, only print what would be deleted without deleting.

    Returns:
        None
    """
    target_path = Path(target).resolve()

    if not target_path.exists():
        Logger.Info(f"Target does not exist, nothing to clean: {target_path}")
        return

    if dry_run:
        Logger.Info(f"Would remove: {target_path}")
        return

    if target_path.is_file():
        try:
            target_path.unlink()
            Logger.Info(f"File removed: {target_path}")
        except Exception as e:
            Logger.Error(f"Failed to remove file: {target_path}. Reason: {e}")
    elif target_path.is_dir():
        try:
            shutil.rmtree(target_path)
            Logger.Info(f"Directory removed: {target_path}")
        except Exception as e:
            Logger.Error(f"Failed to remove directory: {target_path}. Reason: {e}")
    else:
        Logger.Warn(f"Target is neither a file nor a directory: {target_path}")
