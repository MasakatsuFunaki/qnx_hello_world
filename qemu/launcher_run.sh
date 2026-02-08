#!/bin/bash
# Launcher — delegates to the Python implementation.
# This sh_binary wrapper avoids Python toolchain conflicts when
# Bazel is configured with a cross-compilation --platforms flag.
set -euo pipefail

# Resolve the runfiles directory (works under `bazel run` and standalone)
if [[ -n "${RUNFILES_DIR:-}" ]]; then
    PYROOT="${RUNFILES_DIR}/_main/qemu"
elif [[ -d "${BASH_SOURCE[0]}.runfiles/_main/qemu" ]]; then
    PYROOT="$(cd "${BASH_SOURCE[0]}.runfiles/_main/qemu" && pwd)"
else
    PYROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
fi

export PYTHONPATH="${PYROOT}:${PYTHONPATH:-}"
exec python3 "${PYROOT}/run_qnx_qemu.py" "$@"
