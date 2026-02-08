"""
VM workspace manager — handles the on-disk directory layout used
by mkqnximage and the staging area for the demo binary and test binaries.

Implements IVmWorkspace.
"""

from __future__ import annotations

import shutil
from pathlib import Path
from typing import List

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
        self._copy_executable(binary_path, dest)
        return dest

    def stage_test_binaries(self, test_binary_paths: List[Path]) -> List[Path]:
        """
        Copy all test binaries into a tests/ staging subdirectory.

        Returns the list of staged file paths.
        """
        stage_dir = self._vm_dir / "staging" / "tests"
        stage_dir.mkdir(parents=True, exist_ok=True)

        staged: List[Path] = []
        for src in test_binary_paths:
            dest = stage_dir / src.name
            self._copy_executable(src, dest)
            staged.append(dest)
            ConsoleLogger.info("Staged test", src.name)

        ConsoleLogger.success(f"  Staged {len(staged)} test binaries")
        return staged

    def write_data_files_snippet(
        self,
        binary_name: str,
        staged_path: Path,
        staged_tests: List[Path] | None = None,
    ) -> None:
        """Write the mkqnximage data_files.custom snippet."""
        lines = [
            "# Custom binaries baked into the QNX image",
            f"[uid=0 gid=0 perms=0755] home/root/{binary_name}={staged_path}",
        ]

        if staged_tests:
            lines.append("")
            lines.append("# Test binaries")
            for test_path in staged_tests:
                lines.append(
                    f"[uid=0 gid=0 perms=0755] home/root/tests/{test_path.name}={test_path}"
                )

        snippet = self._vm_dir / "local" / "snippets" / "data_files.custom"
        snippet.write_text("\n".join(lines) + "\n")

    # ── Private ─────────────────────────────────────────────────────────

    @staticmethod
    def _copy_executable(src: Path, dest: Path) -> None:
        """Copy a binary into the staging area, replacing any existing file."""
        if dest.exists():
            dest.chmod(0o755)
            dest.unlink()
        shutil.copy2(src, dest)
        dest.chmod(0o755)
