"""
Process manager — PID-file-based lifecycle for the QEMU process.

Implements IProcessManager.
"""

from __future__ import annotations

import os
import signal
from pathlib import Path

from .interfaces import IProcessManager
from .logger import ConsoleLogger


class ProcessManager(IProcessManager):
    """Manage a QEMU process through its PID file."""

    def __init__(self, pid_file: Path) -> None:
        self._pid_file = pid_file

    # ── IProcessManager ─────────────────────────────────────────────────

    def pid_file(self) -> Path:
        return self._pid_file

    def is_running(self) -> bool:
        pid = self._read_pid()
        if pid is None:
            return False
        return self._process_alive(pid)

    def stop(self) -> bool:
        pid = self._read_pid()
        if pid is None:
            ConsoleLogger.success("No QEMU PID file found. Is the VM running?")
            return False

        if self._process_alive(pid):
            ConsoleLogger.success(f"Stopping QNX QEMU (PID {pid}) ...")
            os.kill(pid, signal.SIGTERM)
            self._remove_pid_file()
            ConsoleLogger.success("Done.")
            return True
        else:
            ConsoleLogger.success(f"QEMU process {pid} is not running (stale PID file).")
            self._remove_pid_file()
            return False

    # ── Private helpers ─────────────────────────────────────────────────

    def _read_pid(self) -> int | None:
        if not self._pid_file.is_file():
            return None
        try:
            return int(self._pid_file.read_text().strip())
        except (ValueError, OSError):
            return None

    @staticmethod
    def _process_alive(pid: int) -> bool:
        try:
            os.kill(pid, 0)
            return True
        except OSError:
            return False

    def _remove_pid_file(self) -> None:
        try:
            self._pid_file.unlink()
        except FileNotFoundError:
            pass
