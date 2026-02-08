# ─────────────────────────────────────────────────────────────────────────────
# QNX QEMU targets (Python)
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

py_library(
    name = "qnx_qemu",
    srcs = glob(["qnx_qemu/**/*.py"]),
    imports = ["."],
)

py_binary(
    name = "run_qemu",
    srcs = ["run_qnx_qemu.py"],
    main = "run_qnx_qemu.py",
    args = ["$(location //src:hello_world)"],
    data = ["//src:hello_world"],
    deps = [":qnx_qemu"],
    # This target needs the binary cross-compiled for QNX x86_64.
    # Always invoke with: --config=qnx_x86_64
    # e.g.:  bazel run //qemu:run_qemu --config=qnx_x86_64
)

py_binary(
    name = "stop_qemu",
    srcs = ["stop_qnx_qemu.py"],
    main = "stop_qnx_qemu.py",
    deps = [":qnx_qemu"],
)

py_binary(
    name = "clean_qemu",
    srcs = ["clean_qnx_qemu.py"],
    main = "clean_qnx_qemu.py",
    deps = [":qnx_qemu"],
)
