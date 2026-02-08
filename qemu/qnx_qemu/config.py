"""
Configuration layer — centralises all tuneable parameters.

Values are read from environment variables with sensible defaults,
following the same contract as the original shell scripts.
"""

from __future__ import annotations

import os
import platform
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List


# ── Architecture mapping ─────────────────────────────────────────────────────
_ARCH_MAP: Dict[str, Dict[str, Any]] = {
    "x86_64": {
        "qemu_system": "qemu-system-x86_64",
        "mkqnximage_arch": "x86_64",
        "bazel_config": "qnx_x86_64",
        "machine": None,           # x86_64 has a sensible default
        "disk_interface": "ide",
        "host_match": "x86_64",    # host arch that supports KVM for this target
    },
    "aarch64": {
        "qemu_system": "qemu-system-aarch64",
        "mkqnximage_arch": "aarch64le",
        "bazel_config": "qnx_aarch64",
        "machine": "virt-4.2",          # matches QNX's official runimage script
        "disk_interface": "virtio-blk",  # virtio-blk-device over MMIO (no PCI needed)
        "host_match": "aarch64",
    },
}


@dataclass(frozen=True)
class QemuConfig:
    """Immutable configuration for a QNX QEMU session."""

    arch: str = field(default_factory=lambda: os.environ.get("QNX_QEMU_ARCH", "x86_64"))
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

    def __post_init__(self) -> None:
        if self.arch not in _ARCH_MAP:
            supported = ", ".join(sorted(_ARCH_MAP))
            raise ValueError(
                f"Unsupported architecture '{self.arch}'. "
                f"Supported: {supported}. "
                f"Set QNX_QEMU_ARCH to one of these values."
            )

    @property
    def qemu_system_binary(self) -> str:
        """Return the QEMU system emulator binary name for this arch."""
        return _ARCH_MAP[self.arch]["qemu_system"]

    @property
    def mkqnximage_arch(self) -> str:
        """Return the --arch value for mkqnximage."""
        return _ARCH_MAP[self.arch]["mkqnximage_arch"]

    @property
    def bazel_config(self) -> str:
        """Return the Bazel --config name for this arch."""
        return _ARCH_MAP[self.arch]["bazel_config"]

    @property
    def machine(self) -> str | None:
        """Return the QEMU -machine type, or None if not needed."""
        return _ARCH_MAP[self.arch]["machine"]

    @property
    def disk_interface(self) -> str:
        """Return the QEMU disk interface (ide, virtio, none)."""
        return _ARCH_MAP[self.arch]["disk_interface"]

    @property
    def is_cross_arch(self) -> bool:
        """True if target arch differs from host arch (no KVM possible)."""
        host = platform.machine()  # e.g. 'x86_64', 'aarch64'
        return host != _ARCH_MAP[self.arch]["host_match"]
