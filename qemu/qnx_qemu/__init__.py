"""
qnx_qemu — Python package for managing QNX QEMU virtual machines.

Architecture follows C++ design patterns:
  - Interfaces (ABCs)     : pure-virtual-style contracts
  - Builder               : QemuCommandBuilder for CLI arg construction
  - Strategy              : KVM vs software emulation acceleration
  - Facade                : QemuManager orchestrates the full workflow
  - Layered design        : config → infrastructure → domain → application
"""

__version__ = "1.0.0"
