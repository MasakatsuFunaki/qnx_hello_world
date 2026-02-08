#!/usr/bin/env python3
"""
run_qnx_qemu.py — Build a QNX QEMU image with the demo binary
                   and all test binaries baked into the filesystem,
                   then launch QEMU.

Usage (via Bazel):
    bazel run //qemu:run_qemu --config=qnx_x86_64
    bazel run //qemu:run_qemu --config=qnx_aarch64

The architecture is auto-detected from the --config flag.

After launch, SSH in from another terminal:
    ssh -o StrictHostKeyChecking=no -p 2222 root@localhost
    /data/home/root/hello_world

Test binaries are auto-discovered from the Bazel runfiles tree
and placed under /data/home/root/tests/ inside the QNX image.

Press Ctrl-A X to exit QEMU console.
"""

from __future__ import annotations

import sys

from qnx_qemu.config import QemuConfig
from qnx_qemu.logger import ConsoleLogger
from qnx_qemu.manager import QemuManager


def main() -> None:
    if len(sys.argv) < 2:
        ConsoleLogger.error("No binary path provided.")
        ConsoleLogger.error_hint(f"Usage: {sys.argv[0]} <path-to-qnx-binary>")
        sys.exit(1)

    binary_arg = sys.argv[1]

    # Parse --arch flag (injected by Bazel select() based on --config)
    arch: str | None = None
    if "--arch" in sys.argv:
        idx = sys.argv.index("--arch")
        if idx + 1 < len(sys.argv):
            arch = sys.argv[idx + 1]

    try:
        config = QemuConfig(arch=arch) if arch else QemuConfig()
        manager = QemuManager(config=config)
        manager.run(binary_arg)
    except (FileNotFoundError, EnvironmentError, RuntimeError, ValueError) as exc:
        ConsoleLogger.error(str(exc))
        sys.exit(1)


if __name__ == "__main__":
    main()
