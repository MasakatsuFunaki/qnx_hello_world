"""
Path resolution layer — locates workspace root, QNX SDK,
the cross-compiled binary, test binaries, and the mkqnximage tool.

Implements IPathResolver.
"""

from __future__ import annotations

import os
import stat
from pathlib import Path
from typing import List, Optional

from .config import QemuConfig
from .interfaces import IPathResolver
from .logger import ConsoleLogger


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
            f"Make sure to build first with --config={self._config.bazel_config}"
        )

    def qnx_sdk_root(self) -> Path:
        sdk = self._config.qnx_root
        if not sdk.is_dir():
            raise FileNotFoundError(
                f"QNX SDP not found at {sdk}. "
                f"Set QNX_ROOT or install the SDP to ~/qnx800"
            )
        return sdk

    def discover_test_binaries(self) -> List[Path]:
        """
        Auto-discover cross-compiled test binaries.

        Search order:
        1. RUNFILES_DIR/_main/tests/  (Bazel runfiles tree)
        2. <workspace>/bazel-bin/tests/  (direct build output)

        Returns all executable files found under tests/ subdirectories.
        """
        search_roots: List[Path] = []

        runfiles = self._config.runfiles_dir
        if runfiles:
            search_roots.append(Path(runfiles) / "_main" / "tests")
            search_roots.append(Path(runfiles) / "tests")

        ws = self.workspace_root()
        search_roots.append(ws / "bazel-bin" / "tests")

        found: List[Path] = []
        seen_names: set[str] = set()

        for root in search_roots:
            if not root.is_dir():
                continue
            for dirpath, _dirnames, filenames in os.walk(root):
                for fname in sorted(filenames):
                    fpath = Path(dirpath) / fname
                    # Skip non-executable files, .so/.a libs, .py, etc.
                    if not self._is_executable_binary(fpath):
                        continue
                    # Deduplicate by name (same binary in runfiles and bazel-bin)
                    if fname not in seen_names:
                        seen_names.add(fname)
                        found.append(fpath.resolve())
            # If we found binaries in this root, don't search lower-priority roots
            if found:
                break

        ConsoleLogger.info("Tests found", str(len(found)))
        return sorted(found, key=lambda p: p.name)

    @staticmethod
    def _is_executable_binary(path: Path) -> bool:
        """Return True if path looks like a compiled executable (not a lib/script)."""
        # Must be a regular file
        if not path.is_file():
            return False
        # Skip common non-binary extensions
        suffixes = path.suffixes
        skip_extensions = {
            ".py", ".sh", ".so", ".a", ".o", ".d", ".h", ".hpp",
            ".cpp", ".cc", ".txt", ".md", ".json", ".params",
            ".runfiles", ".runfiles_manifest",
        }
        if any(s in skip_extensions for s in suffixes):
            return False
        # Skip filenames that are clearly not test binaries
        if path.name.startswith(".") or path.name.startswith("_"):
            return False
        # Must have executable bit set
        try:
            return bool(path.stat().st_mode & stat.S_IXUSR)
        except OSError:
            return False

    def mkqnximage_path(self) -> Path:
        path = self.qnx_sdk_root() / "host" / "common" / "bin" / "mkqnximage"
        if not path.exists():
            raise FileNotFoundError(f"mkqnximage not found at {path}")
        return path
