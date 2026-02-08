# ─────────────────────────────────────────────────────────────────────────────
# QNX QEMU targets (Python)
#
# Build & run:
#   bazel run //qemu:run_qemu --config=qnx_x86_64
#   bazel run //qemu:run_qemu --config=qnx_aarch64
#
# The architecture is auto-detected from the --config flag.
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
    args = [
        "$(location //src:hello_world)",
    ] + select({
        "@platforms//cpu:x86_64": ["--arch", "x86_64"],
        "@platforms//cpu:aarch64": ["--arch", "aarch64"],
    }),
    data = [
        "//src:hello_world",
        "//tests:all_tests",
    ],
    deps = [":qnx_qemu"],
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
