"""
Acceleration strategies — C++ Strategy pattern.

Two concrete strategies:
  • KvmStrategy       — uses /dev/kvm with host CPU pass-through
  • EmulationStrategy — pure software emulation

KVM is only used when the target arch matches the host arch.
"""

from __future__ import annotations

import os
from pathlib import Path

from .interfaces import AccelerationFlags, IAccelerationStrategy


class KvmStrategy(IAccelerationStrategy):
    """Use KVM hardware acceleration (requires /dev/kvm writable)."""

    def resolve(self) -> AccelerationFlags:
        return AccelerationFlags(
            qemu_args=["--enable-kvm", "--cpu", "host"],
            description="Using KVM acceleration",
        )


class EmulationStrategy(IAccelerationStrategy):
    """Fall back to pure software emulation."""

    def __init__(self, target_arch: str = "x86_64") -> None:
        self._target_arch = target_arch

    def resolve(self) -> AccelerationFlags:
        # Pick a sensible CPU model per target architecture
        cpu = {
            "x86_64": "max",
            "aarch64": "cortex-a57",
        }.get(self._target_arch, "max")

        return AccelerationFlags(
            qemu_args=["--cpu", cpu],
            description=f"KVM not available for {self._target_arch} on this host, using emulation (slower)",
        )


def detect_best_strategy(target_arch: str = "x86_64", is_cross_arch: bool = False) -> IAccelerationStrategy:
    """Factory: return KvmStrategy if same-arch and /dev/kvm is writable, else EmulationStrategy."""
    if is_cross_arch:
        return EmulationStrategy(target_arch)

    kvm_path = Path("/dev/kvm")
    if kvm_path.exists() and os.access(str(kvm_path), os.W_OK):
        return KvmStrategy()
    return EmulationStrategy(target_arch)
