"""
Path resolution layer — locates workspace root, QNX SDK,
the cross-compiled binary, and the mkqnximage tool.

Implements IPathResolver.
"""

from __future__ import annotations

from pathlib import Path
from typing import Optional

from .config import QemuConfig
from .interfaces import IPathResolver


class PathResolver(IPathResolver):
    """Concrete path resolver using Bazel environment + filesystem probing."""

    def __init__(self, config: QemuConfig, script_dir: Optional[Path] = None) -> None:
        self._config = config
        self._script_dir = script_dir or Path(__file__).resolve().parent.parent
        self._workspace: Optional[Path] = None

    # ── IPathResolver ───────────────────────────────────────────────────

    def workspace_root(self) -> Path:
        if self._workspace is None:
            if self._config.build_workspace_directory:
                self._workspace = Path(self._config.build_workspace_directory).resolve()
            else:
                # Fallback: assume script lives at <workspace>/qemu/qnx_qemu/
                self._workspace = self._script_dir.parent.resolve()
            if not self._workspace.is_dir():
                raise FileNotFoundError(
                    f"Workspace root not found: {self._workspace}"
                )
        return self._workspace

    def resolve_binary(self, binary_arg: str) -> Path:
        """
        Search order mirrors the original shell script:
        1. Absolute / cwd-relative path
        2. RUNFILES_DIR/<arg>
        3. RUNFILES_DIR/_main/<arg>
        4. <workspace>/bazel-bin/<arg>
        5. <workspace>/bazel-bin/src/hello_world  (convenience fallback)
        """
        candidates = [Path(binary_arg)]

        runfiles = self._config.runfiles_dir
        if runfiles:
            candidates.append(Path(runfiles) / binary_arg)
            candidates.append(Path(runfiles) / "_main" / binary_arg)

        ws = self.workspace_root()
        candidates.append(ws / "bazel-bin" / binary_arg)
        candidates.append(ws / "bazel-bin" / "src" / "hello_world")

        for candidate in candidates:
            if candidate.is_file():
                return candidate.resolve()

        searched = ", ".join(str(c) for c in candidates)
        raise FileNotFoundError(
            f"Binary '{binary_arg}' not found.  Searched: {searched}\n"
            f"Make sure to build first with --config=qnx_x86_64"
        )

    def qnx_sdk_root(self) -> Path:
        sdk = self._config.qnx_root
        if not sdk.is_dir():
            raise FileNotFoundError(
                f"QNX SDP not found at {sdk}. "
                f"Set QNX_ROOT or install the SDP to ~/qnx800"
            )
        return sdk

    def mkqnximage_path(self) -> Path:
        path = self.qnx_sdk_root() / "host" / "common" / "bin" / "mkqnximage"
        if not path.exists():
            raise FileNotFoundError(f"mkqnximage not found at {path}")
        return path
