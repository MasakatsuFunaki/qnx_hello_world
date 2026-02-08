"""
Configuration layer — centralises all tuneable parameters.

Values are read from environment variables with sensible defaults,
following the same contract as the original shell scripts.
"""

from __future__ import annotations

import os
from dataclasses import dataclass, field
from pathlib import Path


@dataclass(frozen=True)
class QemuConfig:
    """Immutable configuration for a QNX QEMU session."""

    ssh_port: int = field(default_factory=lambda: int(os.environ.get("QNX_QEMU_SSH_PORT", "2222")))
    ram: str = field(default_factory=lambda: os.environ.get("QNX_QEMU_RAM", "1G"))
    smp: int = field(default_factory=lambda: int(os.environ.get("QNX_QEMU_SMP", "2")))
    qnx_root: Path = field(default_factory=lambda: Path(os.environ.get("QNX_ROOT", Path.home() / "qnx800")))
    vm_dir_name: str = ".qnx_qemu_vm"

    # Bazel-specific environment variables (may be empty)
    build_workspace_directory: str = field(
        default_factory=lambda: os.environ.get("BUILD_WORKSPACE_DIRECTORY", "")
    )
    runfiles_dir: str = field(
        default_factory=lambda: os.environ.get("RUNFILES_DIR", "")
    )
