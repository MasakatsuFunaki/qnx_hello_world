# ─────────────────────────────────────────────────────────────────────────────
# Aggregated test binaries target
#
# This filegroup collects ALL cc_binary test targets from every subdirectory.
# The QEMU runner uses this single target to stage all test binaries into
# the QNX image — no matter how many tests are added to subdirectories.
#
# To add a new test category:
#   1. Create a new subdirectory with its own BUILD file
#   2. Add the cc_binary targets with visibility = ["//tests:__pkg__"]
#   3. Add the target(s) to the `all_tests` filegroup below
#
# Build all tests for QNX:
#   bazel build //tests:all_tests --config=qnx_x86_64
#   bazel build //tests:all_tests --config=qnx_aarch64
# ─────────────────────────────────────────────────────────────────────────────

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all_tests",
    srcs = [
        # build_features
        "//tests/build_features:copts_test",
        "//tests/build_features:defines_test",
        "//tests/build_features:include_test",

        # cpp_features
        "//tests/cpp_features:cpp17_types",
        "//tests/cpp_features:exceptions",
        "//tests/cpp_features:inheritance",
        "//tests/cpp_features:lambdas",
        "//tests/cpp_features:modern_cpp",
        "//tests/cpp_features:smart_pointers",
        "//tests/cpp_features:templates",

        # lib_chain
        "//tests/lib_chain:chain_test",

        # lib_header_only
        "//tests/lib_header_only:header_only_test",

        # lib_shared
        "//tests/lib_shared:shared_lib_test",

        # lib_static
        "//tests/lib_static:static_lib_test",

        # qnx_specific
        "//tests/qnx_specific:qnx_api_test",
        "//tests/qnx_specific:timer_pulse_test",

        # stl_containers
        "//tests/stl_containers:stl_test",

        # threading
        "//tests/threading:threading_test",
    ],
)
