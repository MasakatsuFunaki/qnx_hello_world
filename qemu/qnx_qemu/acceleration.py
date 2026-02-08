"""
Acceleration strategies — C++ Strategy pattern.

Two concrete strategies:
  • KvmStrategy       — uses /dev/kvm with host CPU pass-through
  • EmulationStrategy — pure software emulation with qemu64 CPU
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

    def resolve(self) -> AccelerationFlags:
        return AccelerationFlags(
            qemu_args=["--cpu", "max"],
            description="WARNING: KVM not available, emulation will be slower",
        )


def detect_best_strategy() -> IAccelerationStrategy:
    """Factory: return KvmStrategy if /dev/kvm is writable, else EmulationStrategy."""
    kvm_path = Path("/dev/kvm")
    if kvm_path.exists() and os.access(str(kvm_path), os.W_OK):
        return KvmStrategy()
    return EmulationStrategy()
