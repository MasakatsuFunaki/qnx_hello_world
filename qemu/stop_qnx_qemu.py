#!/usr/bin/env python3
"""
stop_qnx_qemu.py — Stop the running QNX QEMU instance.

Usage (via Bazel):
    bazel run //qemu:stop_qemu
"""

from __future__ import annotations

import sys

from qnx_qemu.logger import ConsoleLogger
from qnx_qemu.manager import QemuManager


def main() -> None:
    try:
        manager = QemuManager()
        manager.stop()
    except (FileNotFoundError, EnvironmentError, RuntimeError) as exc:
        ConsoleLogger.error(str(exc))
        sys.exit(1)


if __name__ == "__main__":
    main()
