# BUILD file for static library test
# Tests: cc_library (static), deps, hdrs, includes

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//tests:__pkg__", "//qemu:__pkg__"])

cc_library(
    name = "mathlib",
    srcs = ["mathlib.cpp"],
    hdrs = ["mathlib.h"],
    copts = ["-std=c++17"],
)

cc_binary(
    name = "static_lib_test",
    srcs = ["static_lib_test.cpp"],
    copts = ["-std=c++17"],
    deps = [":mathlib"],
)
