"""
QemuManager — Facade (C++ Facade pattern).

Orchestrates the full run / stop / clean workflows by composing
lower-level services through their abstract interfaces.
"""

from __future__ import annotations

import os
import shutil
from pathlib import Path
from typing import Optional

from .acceleration import detect_best_strategy
from .config import QemuConfig
from .image_builder import MkqnximageBuilder
from .interfaces import (
    IAccelerationStrategy,
    IImageBuilder,
    IPathResolver,
    IProcessManager,
    ISshKeyProvider,
    IVmWorkspace,
)
from .logger import ConsoleLogger
from .path_resolver import PathResolver
from .process import ProcessManager
from .qemu_command import QemuCommandBuilder
from .ssh import SshKeyProvider
from .vm_workspace import VmWorkspace


class QemuManager:
    """
    High-level facade that composes all subsystems.

    Dependency injection: callers can supply custom implementations
    of every interface for testing or alternative configurations.
    """

    def __init__(
        self,
        config: Optional[QemuConfig] = None,
        path_resolver: Optional[IPathResolver] = None,
        ssh_provider: Optional[ISshKeyProvider] = None,
        image_builder: Optional[IImageBuilder] = None,
        acceleration: Optional[IAccelerationStrategy] = None,
        process_manager: Optional[IProcessManager] = None,
        vm_workspace: Optional[IVmWorkspace] = None,
    ) -> None:
        self._config = config or QemuConfig()

        # ── Wire up defaults using concrete implementations ─────────
        self._paths: IPathResolver = path_resolver or PathResolver(self._config)
        self._ssh: ISshKeyProvider = ssh_provider or SshKeyProvider()

        ws_root = self._paths.workspace_root()
        _vm_dir = ws_root / self._config.vm_dir_name

        self._vm: IVmWorkspace = vm_workspace or VmWorkspace(_vm_dir)
        self._proc: IProcessManager = process_manager or ProcessManager(
            _vm_dir / "output" / "qemu.pid"
        )
        self._accel: IAccelerationStrategy = acceleration or detect_best_strategy()
        self._builder: IImageBuilder = image_builder or MkqnximageBuilder(
            mkqnximage=self._paths.mkqnximage_path(),
            qnx_sdk_root=self._paths.qnx_sdk_root(),
        )

    # ── Public workflow methods ─────────────────────────────────────────

    def run(self, binary_arg: str) -> None:
        """
        Full run workflow:
        1. Resolve paths
        2. Prepare VM workspace & stage binary
        3. Build QNX image
        4. Launch QEMU
        """
        self._check_qemu_installed()

        binary_path = self._paths.resolve_binary(binary_arg)
        binary_name = binary_path.name

        ConsoleLogger.banner("QNX QEMU Launcher")
        ConsoleLogger.info("Binary", str(binary_path))
        ConsoleLogger.info("Name", binary_name)

        ssh_pubkey = self._ssh.public_key_path()
        ConsoleLogger.info("SSH key", str(ssh_pubkey))

        vm_dir = self._vm.vm_dir()
        ConsoleLogger.info("VM dir", str(vm_dir))
        ConsoleLogger.rule()

        # Prepare directories and stage the binary
        self._vm.prepare()
        staged = self._vm.stage_binary(binary_path, binary_name)  # type: ignore[attr-defined]
        self._vm.write_data_files_snippet(binary_name, staged)    # type: ignore[attr-defined]

        # Build the QNX image
        artifacts = self._builder.build(
            binary_path=binary_path,
            binary_name=binary_name,
            ssh_pubkey=ssh_pubkey,
            vm_dir=vm_dir,
        )

        # Resolve acceleration
        accel = self._accel.resolve()

        ConsoleLogger.step(2, 2, "Launching QNX in QEMU")
        ConsoleLogger.ssh_instructions(self._config.ssh_port, binary_name)

        if "KVM" in accel.description:
            ConsoleLogger.info("Accel", accel.description)
        else:
            ConsoleLogger.warning(accel.description)

        # Build and exec the QEMU command
        cmd = (
            QemuCommandBuilder()
            .set_smp(self._config.smp)
            .set_acceleration(accel)
            .set_memory(self._config.ram)
            .set_kernel(artifacts.ifs_image)
            .set_disk(artifacts.disk_image)
            .set_network(self._config.ssh_port)
            .set_rng()
            .set_serial()
            .set_pidfile(self._proc.pid_file())
            .build()
        )

        # Replace current process (like shell `exec`)
        os.execvp(cmd[0], cmd)

    def stop(self) -> None:
        """Stop a running QEMU instance."""
        self._proc.stop()

    def clean(self) -> None:
        """Stop (if running) then wipe the VM workspace."""
        self._proc.stop()
        self._vm.clean()

    # ── Private helpers ─────────────────────────────────────────────────

    @staticmethod
    def _check_qemu_installed() -> None:
        if shutil.which("qemu-system-x86_64") is None:
            raise EnvironmentError(
                "qemu-system-x86_64 not found in PATH.\n"
                "Install it with:  sudo apt install qemu-system-x86"
            )
