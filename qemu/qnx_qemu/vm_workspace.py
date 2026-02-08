"""
VM workspace manager — handles the on-disk directory layout used
by mkqnximage and the staging area for the demo binary.

Implements IVmWorkspace.
"""

from __future__ import annotations

import shutil
from pathlib import Path

from .interfaces import IVmWorkspace
from .logger import ConsoleLogger


class VmWorkspace(IVmWorkspace):
    """Manages the .qnx_qemu_vm/ directory tree."""

    def __init__(self, vm_dir: Path) -> None:
        self._vm_dir = vm_dir

    # ── IVmWorkspace ────────────────────────────────────────────────────

    def vm_dir(self) -> Path:
        return self._vm_dir

    def prepare(self) -> None:
        self._vm_dir.mkdir(parents=True, exist_ok=True)
        (self._vm_dir / "local" / "snippets").mkdir(parents=True, exist_ok=True)

    def clean(self) -> None:
        if self._vm_dir.is_dir():
            ConsoleLogger.success(f"Removing {self._vm_dir} ...")
            shutil.rmtree(self._vm_dir)
            ConsoleLogger.success("Done.")
        else:
            ConsoleLogger.success(f"Nothing to clean ({self._vm_dir} does not exist).")

    def exists(self) -> bool:
        return self._vm_dir.is_dir()

    # ── Staging helpers (used by the image builder) ─────────────────────

    def stage_binary(self, binary_path: Path, binary_name: str) -> Path:
        """
        Copy the compiled binary into the staging area.

        Returns the staged file path.
        """
        stage_dir = self._vm_dir / "staging" / "bin"
        stage_dir.mkdir(parents=True, exist_ok=True)
        dest = stage_dir / binary_name
        # Remove existing file first (may be read-only from a previous run)
        if dest.exists():
            dest.chmod(0o755)
            dest.unlink()
        shutil.copy2(binary_path, dest)
        dest.chmod(0o755)
        return dest

    def write_data_files_snippet(self, binary_name: str, staged_path: Path) -> None:
        """Write the mkqnximage data_files.custom snippet."""
        snippet = self._vm_dir / "local" / "snippets" / "data_files.custom"
        snippet.write_text(
            "# Custom demo binary baked into the QNX image\n"
            f"[uid=0 gid=0 perms=0755] home/root/{binary_name}={staged_path}\n"
        )
