# BUILD file for testing build features: defines, copts, select(), includes

package(default_visibility = ["//tests:__pkg__", "//qemu:__pkg__"])

# ─────────────────────────────────────────────────────────────────────────────
# Test 1: defines and select()
# ─────────────────────────────────────────────────────────────────────────────
cc_binary(
    name = "defines_test",
    srcs = ["defines_test.cpp"],
    copts = ["-std=c++17"],
    defines = [
        "TEST_DEFINE_ONE",
        "TEST_DEFINE_TWO=42",
        'APP_VERSION=\\"1.0.0\\"',
    ] + select({
        "@platforms//os:qnx": ["BUILD_FOR_QNX"],
        "@platforms//os:linux": ["BUILD_FOR_LINUX"],
        "//conditions:default": [],
    }),
)

# ─────────────────────────────────────────────────────────────────────────────
# Test 2: strip_include_prefix and header-only library with custom include path
# ─────────────────────────────────────────────────────────────────────────────
cc_library(
    name = "myapi",
    hdrs = ["include/myapi/myapi.h"],
    strip_include_prefix = "include",
)

cc_binary(
    name = "include_test",
    srcs = ["include_test.cpp"],
    copts = ["-std=c++17"],
    deps = [":myapi"],
)

# ─────────────────────────────────────────────────────────────────────────────
# Test 3: copts – specific compiler flags
# ─────────────────────────────────────────────────────────────────────────────
cc_binary(
    name = "copts_test",
    srcs = ["copts_test.cpp"],
    copts = [
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Werror",
    ],
)
