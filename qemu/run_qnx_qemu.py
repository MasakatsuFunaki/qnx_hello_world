#!/usr/bin/env python3
"""
run_qnx_qemu.py — Build a QNX x86_64 QEMU image with the demo binary
                   baked into the filesystem, then launch QEMU.

Usage (via Bazel):
    bazel run //qemu:run_qemu --config=qnx_x86_64

After launch, SSH in from another terminal:
    ssh -o StrictHostKeyChecking=no -p 2222 root@localhost
    /data/home/root/hello_world

Press Ctrl-A X to exit QEMU console.
"""

from __future__ import annotations

import sys

from qnx_qemu.logger import ConsoleLogger
from qnx_qemu.manager import QemuManager


def main() -> None:
    if len(sys.argv) < 2:
        ConsoleLogger.error("No binary path provided.")
        ConsoleLogger.error_hint(f"Usage: {sys.argv[0]} <path-to-qnx-binary>")
        sys.exit(1)

    binary_arg = sys.argv[1]

    try:
        manager = QemuManager()
        manager.run(binary_arg)
    except (FileNotFoundError, EnvironmentError, RuntimeError) as exc:
        ConsoleLogger.error(str(exc))
        sys.exit(1)


if __name__ == "__main__":
    main()
