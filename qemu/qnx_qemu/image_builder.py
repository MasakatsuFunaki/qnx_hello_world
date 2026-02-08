"""
QNX image builder — wraps ``mkqnximage`` to produce IFS + disk images.

Implements IImageBuilder.
"""

from __future__ import annotations

import os
import subprocess
from pathlib import Path

from .interfaces import BuildArtifacts, IImageBuilder
from .logger import ConsoleLogger
from .vm_workspace import VmWorkspace


class MkqnximageBuilder(IImageBuilder):
    """Build a QNX QEMU image using the QNX SDP's mkqnximage tool."""

    def __init__(self, mkqnximage: Path, qnx_sdk_root: Path, arch: str = "x86_64") -> None:
        self._mkqnximage = mkqnximage
        self._qnx_sdk_root = qnx_sdk_root
        self._arch = arch

    # ── IImageBuilder ──────────────────────────────────────────────────

    def build(
        self,
        binary_path: Path,
        binary_name: str,
        ssh_pubkey: Path,
        vm_dir: Path,
    ) -> BuildArtifacts:
        # Ensure QNX environment variables are set
        self._source_qnx_env()

        args = [
            str(self._mkqnximage),
            "--type=qemu",
            f"--arch={self._arch}",
            f"--ssh-ident={ssh_pubkey}",
            "--noprompt",
            "--force",
            "--clean",
            "--build",
        ]

        ConsoleLogger.step(1, 2, "Building QNX QEMU image with mkqnximage")
        ConsoleLogger.success(f"  {' '.join(args)}\n")

        subprocess.run(args, cwd=str(vm_dir), check=True)

        ifs_image = vm_dir / "output" / "ifs.bin"
        disk_image = vm_dir / "output" / "disk-qemu.vmdk"

        if not ifs_image.is_file():
            raise RuntimeError(
                f"IFS image not found at {ifs_image} — mkqnximage build may have failed."
            )
        if not disk_image.is_file():
            raise RuntimeError(
                f"Disk image not found at {disk_image} — mkqnximage build may have failed."
            )

        return BuildArtifacts(ifs_image=ifs_image, disk_image=disk_image)

    # ── Private ────────────────────────────────────────────────────────

    def _source_qnx_env(self) -> None:
        """Ensure QNX_TARGET and QNX_HOST are set (source qnxsdp-env.sh)."""
        if os.environ.get("QNX_TARGET"):
            return  # already sourced

        env_script = self._qnx_sdk_root / "qnxsdp-env.sh"
        if not env_script.is_file():
            raise FileNotFoundError(
                f"QNX environment script not found: {env_script}"
            )

        ConsoleLogger.info("QNX env", f"Sourcing {env_script} …")

        # Source the script in a sub-shell and capture the resulting env
        result = subprocess.run(
            ["bash", "-c", f"source '{env_script}' && env"],
            capture_output=True,
            text=True,
            check=True,
        )
        for line in result.stdout.splitlines():
            key, _, value = line.partition("=")
            if key:
                os.environ[key] = value

        ConsoleLogger.info("QNX_HOST", os.environ.get("QNX_HOST", "?"))
        ConsoleLogger.info("QNX_TARGET", os.environ.get("QNX_TARGET", "?"))
