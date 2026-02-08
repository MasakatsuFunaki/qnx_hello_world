"""
QEMU command builder — C++ Builder pattern.

Constructs the full ``qemu-system-x86_64`` argument list step by step,
keeping each concern (CPU, memory, disks, networking, …) isolated.
"""

from __future__ import annotations

from pathlib import Path
from typing import List

from .interfaces import AccelerationFlags


class QemuCommandBuilder:
    """
    Fluent builder that assembles the QEMU CLI invocation.

    Usage::

        cmd = (
            QemuCommandBuilder()
            .set_smp(2)
            .set_acceleration(accel_flags)
            .set_memory("1G")
            .set_kernel(ifs_image)
            .set_disk(disk_image)
            .set_network(ssh_port=2222)
            .set_rng()
            .set_serial()
            .set_pidfile(pid_path)
            .build()
        )
    """

    def __init__(self) -> None:
        self._args: List[str] = ["qemu-system-x86_64"]

    # ── Builder methods (each returns self for chaining) ────────────────

    def set_smp(self, cores: int) -> QemuCommandBuilder:
        self._args += ["-smp", str(cores)]
        return self

    def set_acceleration(self, flags: AccelerationFlags) -> QemuCommandBuilder:
        self._args += flags.qemu_args
        return self

    def set_memory(self, ram: str) -> QemuCommandBuilder:
        self._args += ["-m", ram]
        return self

    def set_kernel(self, ifs_image: Path) -> QemuCommandBuilder:
        self._args += ["-kernel", str(ifs_image)]
        return self

    def set_disk(self, disk_image: Path) -> QemuCommandBuilder:
        self._args += ["-drive", f"file={disk_image},if=ide,id=drv0"]
        return self

    def set_network(self, ssh_port: int) -> QemuCommandBuilder:
        self._args += [
            "-netdev", f"user,id=net0,hostfwd=tcp::{ssh_port}-:22",
            "-device", "virtio-net-pci,netdev=net0",
        ]
        return self

    def set_rng(self) -> QemuCommandBuilder:
        self._args += [
            "-object", "rng-random,filename=/dev/urandom,id=rng0",
            "-device", "virtio-rng-pci,rng=rng0",
        ]
        return self

    def set_serial(self) -> QemuCommandBuilder:
        self._args += ["-serial", "mon:stdio", "-nographic"]
        return self

    def set_pidfile(self, pid_path: Path) -> QemuCommandBuilder:
        self._args += ["-pidfile", str(pid_path)]
        return self

    # ── Terminal build ──────────────────────────────────────────────────

    def build(self) -> List[str]:
        """Return the final argument list (including the executable)."""
        return list(self._args)
