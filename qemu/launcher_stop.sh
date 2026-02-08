#!/bin/bash
# Launcher — delegates to the Python implementation.
set -euo pipefail

if [[ -n "${RUNFILES_DIR:-}" ]]; then
    PYROOT="${RUNFILES_DIR}/_main/qemu"
elif [[ -d "${BASH_SOURCE[0]}.runfiles/_main/qemu" ]]; then
    PYROOT="$(cd "${BASH_SOURCE[0]}.runfiles/_main/qemu" && pwd)"
else
    PYROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
fi

export PYTHONPATH="${PYROOT}:${PYTHONPATH:-}"
exec python3 "${PYROOT}/stop_qnx_qemu.py" "$@"
