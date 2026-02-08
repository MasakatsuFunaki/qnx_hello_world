#!/usr/bin/env python3
"""
clean_qnx_qemu.py — Stop (if running) then wipe the QNX QEMU VM directory.

Usage (via Bazel):
    bazel run //qemu:clean_qemu
"""

from __future__ import annotations

import sys

from qnx_qemu.logger import ConsoleLogger
from qnx_qemu.manager import QemuManager


def main() -> None:
    try:
        manager = QemuManager()
        manager.clean()
    except (FileNotFoundError, EnvironmentError, RuntimeError) as exc:
        ConsoleLogger.error(str(exc))
        sys.exit(1)


if __name__ == "__main__":
    main()
