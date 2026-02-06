load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
     "env_entry",
     "env_set",
     "feature",
     "flag_group",
     "flag_set",
     "tool_path")
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@qnx_sdk//:defs.bzl", "QNX_ROOT")

# ─────────────────────────────────────────────────────────────────────────────
# Per-architecture settings
# ─────────────────────────────────────────────────────────────────────────────
_QNX_ROOT = QNX_ROOT

_ARCH_CONFIG = {
    "x86_64": {
        "triple": "x86_64-pc-nto-qnx8.0.0",
        "target_cpu": "x86_64",
        "target_dir": "x86_64",       # under  <qnx>/target/qnx/<target_dir>
        "defines": [
            "-D__QNXNTO__",
            "-D__X86_64__",
            "-D_QNX_SOURCE",
        ],
    },
    "aarch64": {
        "triple": "aarch64-unknown-nto-qnx8.0.0",
        "target_cpu": "aarch64",
        "target_dir": "aarch64le",    # QNX uses "aarch64le" for the dir name
        "defines": [
            "-D__QNXNTO__",
            "-D__aarch64__",
            "-D_QNX_SOURCE",
        ],
    },
}

# ─────────────────────────────────────────────────────────────────────────────
# Rule implementation
# ─────────────────────────────────────────────────────────────────────────────
def _impl(ctx):
    arch = ctx.attr.cpu
    cfg = _ARCH_CONFIG[arch]
    triple = cfg["triple"]
    target_dir = cfg["target_dir"]

    bin_dir = "{}/host/linux/x86_64/usr/bin".format(_QNX_ROOT)

    tool_paths = [
        tool_path(name = "gcc",     path = "{}/{}-gcc".format(bin_dir, triple)),
        tool_path(name = "ld",      path = "{}/{}-ld".format(bin_dir, triple)),
        tool_path(name = "ar",      path = "{}/{}-ar".format(bin_dir, triple)),
        tool_path(name = "cpp",     path = "{}/{}-cpp".format(bin_dir, triple)),
        tool_path(name = "gcov",    path = "/bin/false"),
        tool_path(name = "nm",      path = "{}/{}-nm".format(bin_dir, triple)),
        tool_path(name = "objdump", path = "{}/{}-objdump".format(bin_dir, triple)),
        tool_path(name = "strip",   path = "{}/{}-strip".format(bin_dir, triple)),
    ]

    # Compile flags ----------------------------------------------------------
    compile_flags = feature(
        name = "compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.assemble,
                    ACTION_NAMES.preprocess_assemble,
                    ACTION_NAMES.linkstamp_compile,
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                    ACTION_NAMES.cpp_header_parsing,
                    ACTION_NAMES.cpp_module_compile,
                    ACTION_NAMES.cpp_module_codegen,
                    ACTION_NAMES.lto_backend,
                    ACTION_NAMES.clif_match,
                ],
                flag_groups = [
                    flag_group(flags = cfg["defines"]),
                ],
            ),
        ],
    )

    # Link flags -------------------------------------------------------------
    link_flags = feature(
        name = "link_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.cpp_link_executable,
                    ACTION_NAMES.cpp_link_dynamic_library,
                    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
                ],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-L{}/target/qnx/{}/lib".format(_QNX_ROOT, target_dir),
                            "-L{}/target/qnx/{}/usr/lib".format(_QNX_ROOT, target_dir),
                            "-lc++",
                            "-lm",
                        ],
                    ),
                ],
            ),
        ],
    )

    # QNX environment – the compiler needs these env vars to find headers/libs
    qnx_host = "{}/host/linux/x86_64".format(_QNX_ROOT)
    qnx_target = "{}/target/qnx".format(_QNX_ROOT)

    all_compile_actions = [
        ACTION_NAMES.assemble,
        ACTION_NAMES.preprocess_assemble,
        ACTION_NAMES.linkstamp_compile,
        ACTION_NAMES.c_compile,
        ACTION_NAMES.cpp_compile,
        ACTION_NAMES.cpp_header_parsing,
        ACTION_NAMES.cpp_module_compile,
        ACTION_NAMES.cpp_module_codegen,
        ACTION_NAMES.lto_backend,
        ACTION_NAMES.clif_match,
    ]

    all_link_actions = [
        ACTION_NAMES.cpp_link_executable,
        ACTION_NAMES.cpp_link_dynamic_library,
        ACTION_NAMES.cpp_link_nodeps_dynamic_library,
    ]

    qnx_env = feature(
        name = "qnx_env",
        enabled = True,
        env_sets = [
            env_set(
                actions = all_compile_actions + all_link_actions,
                env_entries = [
                    env_entry(key = "QNX_HOST", value = qnx_host),
                    env_entry(key = "QNX_TARGET", value = qnx_target),
                ],
            ),
        ],
    )

    features = [compile_flags, link_flags, qnx_env]

    gcc_version = "12.2.0"

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = "qnx_{}".format(arch),
        host_system_name = "local",
        target_system_name = "qnx",
        target_cpu = cfg["target_cpu"],
        target_libc = "qnx",
        compiler = "gcc",
        abi_version = "qnx",
        abi_libc_version = "qnx",
        tool_paths = tool_paths,
        features = features,
        cxx_builtin_include_directories = [
            "{}/target/qnx/usr/include".format(_QNX_ROOT),
            "{}/target/qnx/usr/include/c++/{}".format(_QNX_ROOT, gcc_version),
            "{}/target/qnx/usr/include/c++/{}/{}".format(_QNX_ROOT, gcc_version, triple),
            "{}/host/linux/x86_64/usr/lib/gcc/{}/{}/include".format(_QNX_ROOT, triple, gcc_version),
            "{}/host/linux/x86_64/usr/lib/gcc/{}/{}/include-fixed".format(_QNX_ROOT, triple, gcc_version),
        ],
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {
        "cpu": attr.string(
            mandatory = True,
            values = ["x86_64", "aarch64"],
            doc = "Target CPU architecture",
        ),
    },
    provides = [CcToolchainConfigInfo],
)
