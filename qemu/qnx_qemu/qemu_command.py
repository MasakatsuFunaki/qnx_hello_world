"""
QEMU command builder — C++ Builder pattern.

Constructs the full ``qemu-system-<arch>`` argument list step by step,
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
            QemuCommandBuilder("qemu-system-x86_64")
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

    def __init__(self, qemu_binary: str = "qemu-system-x86_64") -> None:
        self._args: List[str] = [qemu_binary]

    # ── Builder methods (each returns self for chaining) ────────────────
    def set_machine(self, machine: str | None) -> QemuCommandBuilder:
        if machine:
            self._args += ["-machine", machine]
        return self
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

    def set_disk(self, disk_image: Path, interface: str = "ide") -> QemuCommandBuilder:
        if interface == "virtio-blk":
            # aarch64 virt: virtio-blk-device over MMIO — matches QNX's official
            # runimage script.  No PCI bus required; pairs with devb-virtio.
            self._args += [
                "-drive", f"file={disk_image},if=none,format=vmdk,id=drv0",
                "-device", "virtio-blk-device,drive=drv0",
            ]
        elif interface == "nvme":
            # NVMe over PCIe (requires pci-server in the IFS)
            self._args += [
                "-drive", f"file={disk_image},if=none,format=vmdk,id=drv0",
                "-device", "nvme,serial=qnxdisk0,drive=drv0",
            ]
        elif interface == "ahci":
            # AHCI via ICH9
            self._args += [
                "-device", "ahci,id=ahci0",
                "-drive", f"file={disk_image},if=none,format=vmdk,id=drv0",
                "-device", "ide-hd,drive=drv0,bus=ahci0.0",
            ]
        else:
            self._args += ["-drive", f"file={disk_image},if={interface},id=drv0"]
        return self

    def set_network(self, ssh_port: int, mmio: bool = False) -> QemuCommandBuilder:
        if mmio:
            # aarch64 virt: virtio-net-device over MMIO — no PCI bus needed
            self._args += [
                "-netdev", f"user,id=net0,hostfwd=tcp::{ssh_port}-:22",
                "-device", "virtio-net-device,netdev=net0",
            ]
        else:
            self._args += [
                "-netdev", f"user,id=net0,hostfwd=tcp::{ssh_port}-:22",
                "-device", "virtio-net-pci,netdev=net0",
            ]
        return self

    def set_rng(self, mmio: bool = False) -> QemuCommandBuilder:
        if mmio:
            # aarch64 virt: virtio-rng-device over MMIO — no PCI bus needed
            self._args += [
                "-object", "rng-random,filename=/dev/urandom,id=rng0",
                "-device", "virtio-rng-device,rng=rng0",
            ]
        else:
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
