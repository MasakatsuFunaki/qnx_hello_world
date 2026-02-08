"""
Abstract interfaces — analogous to C++ pure-virtual base classes.

Every domain service implements one of these contracts so that layers
remain decoupled and components can be swapped or mocked independently.
"""

from __future__ import annotations

from abc import ABC, abstractmethod
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional


# ── Value objects (C++ struct equivalents) ───────────────────────────────────

@dataclass(frozen=True)
class BuildArtifacts:
    """Immutable result produced by an image builder."""
    ifs_image: Path
    disk_image: Path


@dataclass(frozen=True)
class AccelerationFlags:
    """Flags determined by an acceleration strategy."""
    qemu_args: List[str]
    description: str


# ── Interface: IPathResolver ────────────────────────────────────────────────

class IPathResolver(ABC):
    """Resolve workspace, binary, and SDK paths."""

    @abstractmethod
    def workspace_root(self) -> Path:
        """Return the resolved workspace root directory."""
        ...

    @abstractmethod
    def resolve_binary(self, binary_arg: str) -> Path:
        """Locate the cross-compiled QNX binary, raising if not found."""
        ...

    @abstractmethod
    def qnx_sdk_root(self) -> Path:
        """Return the QNX SDP installation directory."""
        ...

    @abstractmethod
    def mkqnximage_path(self) -> Path:
        """Return the full path to the mkqnximage tool."""
        ...


# ── Interface: ISshKeyProvider ──────────────────────────────────────────────

class ISshKeyProvider(ABC):
    """Provide an SSH public key for VM access."""

    @abstractmethod
    def public_key_path(self) -> Path:
        """Return the path to an SSH public key, generating one if needed."""
        ...


# ── Interface: IImageBuilder ───────────────────────────────────────────────

class IImageBuilder(ABC):
    """Build a QNX QEMU disk/IFS image."""

    @abstractmethod
    def build(
        self,
        binary_path: Path,
        binary_name: str,
        ssh_pubkey: Path,
        vm_dir: Path,
    ) -> BuildArtifacts:
        """
        Build the QNX image with the demo binary baked in.

        Returns the paths to the IFS and disk images.
        """
        ...


# ── Interface: IAccelerationStrategy ────────────────────────────────────────

class IAccelerationStrategy(ABC):
    """Strategy for choosing QEMU hardware acceleration (C++ Strategy pattern)."""

    @abstractmethod
    def resolve(self) -> AccelerationFlags:
        """Detect the best acceleration mode and return the CLI flags."""
        ...


# ── Interface: IProcessManager ─────────────────────────────────────────────

class IProcessManager(ABC):
    """Manage QEMU process lifecycle via PID files."""

    @abstractmethod
    def is_running(self) -> bool:
        """Check whether a QEMU instance is currently running."""
        ...

    @abstractmethod
    def stop(self) -> bool:
        """
        Stop a running QEMU instance.

        Returns True if a process was stopped, False if nothing was running.
        """
        ...

    @abstractmethod
    def pid_file(self) -> Path:
        """Return the path to the PID file."""
        ...


# ── Interface: IVmWorkspace ────────────────────────────────────────────────

class IVmWorkspace(ABC):
    """Manage the on-disk VM workspace directory."""

    @abstractmethod
    def vm_dir(self) -> Path:
        """Return the VM workspace directory."""
        ...

    @abstractmethod
    def prepare(self) -> None:
        """Create the VM directory structure."""
        ...

    @abstractmethod
    def clean(self) -> None:
        """Remove the entire VM workspace."""
        ...

    @abstractmethod
    def exists(self) -> bool:
        """Check whether the VM workspace directory exists."""
        ...
