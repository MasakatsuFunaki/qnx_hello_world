# ─────────────────────────────────────────────────────────────────────────────
# QNX QEMU targets (Python via sh_binary launchers)
#
# sh_binary wrappers invoke python3 directly, avoiding Python toolchain
# conflicts when --platforms is set to a cross-compilation target (QNX).
#
# Build & run:
#   bazel run //qemu:run_qemu --config=qnx_x86_64
#
# Stop:
#   bazel run //qemu:stop_qemu
#
# Clean VM directory:
#   bazel run //qemu:clean_qemu
# ─────────────────────────────────────────────────────────────────────────────

_PYTHON_SRCS = glob(["qnx_qemu/**/*.py"])

sh_binary(
    name = "run_qemu",
    srcs = ["launcher_run.sh"],
    args = ["$(location //src:hello_world)"],
    data = [
        "//src:hello_world",
        "run_qnx_qemu.py",
    ] + _PYTHON_SRCS,
    # This target needs the binary cross-compiled for QNX x86_64.
    # Always invoke with: --config=qnx_x86_64
    # e.g.:  bazel run //qemu:run_qemu --config=qnx_x86_64
)

sh_binary(
    name = "stop_qemu",
    srcs = ["launcher_stop.sh"],
    data = [
        "stop_qnx_qemu.py",
    ] + _PYTHON_SRCS,
)

sh_binary(
    name = "clean_qemu",
    srcs = ["launcher_clean.sh"],
    data = [
        "clean_qnx_qemu.py",
    ] + _PYTHON_SRCS,
)
