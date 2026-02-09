load("@rules_cc//cc/toolchains:cc_toolchain.bzl", "cc_toolchain")
load(":cc_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

# ─────────────────────────────────────────────────────────────────────────────
# Platforms – a "platform" tells Bazel what OS + CPU the binary should target
# ─────────────────────────────────────────────────────────────────────────────

platform(
    name = "qnx_x86_64",
    constraint_values = [
        "@platforms//os:qnx",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "qnx_aarch64",
    constraint_values = [
        "@platforms//os:qnx",
        "@platforms//cpu:aarch64",
    ],
)

# ─────────────────────────────────────────────────────────────────────────────
# Shared filegroups
# ─────────────────────────────────────────────────────────────────────────────

filegroup(name = "empty", srcs = [])

# ═════════════════════════════════════════════════════════════════════════════
#  x86_64 toolchain
# ═════════════════════════════════════════════════════════════════════════════

cc_toolchain_config(
    name = "qnx_x86_64_config",
    cpu = "x86_64",
)

cc_toolchain(
    name = "qnx_x86_64_cc_toolchain",
    all_files = ":empty",
    ar_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":qnx_x86_64_config",
    toolchain_identifier = "qnx_x86_64",
)

toolchain(
    name = "qnx_x86_64_toolchain",
    exec_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@platforms//os:qnx",
        "@platforms//cpu:x86_64",
    ],
    toolchain = ":qnx_x86_64_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

# ═════════════════════════════════════════════════════════════════════════════
#  aarch64 toolchain
# ═════════════════════════════════════════════════════════════════════════════

cc_toolchain_config(
    name = "qnx_aarch64_config",
    cpu = "aarch64",
)

cc_toolchain(
    name = "qnx_aarch64_cc_toolchain",
    all_files = ":empty",
    ar_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":qnx_aarch64_config",
    toolchain_identifier = "qnx_aarch64",
)

toolchain(
    name = "qnx_aarch64_toolchain",
    exec_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@platforms//os:qnx",
        "@platforms//cpu:aarch64",
    ],
    toolchain = ":qnx_aarch64_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)
