# BUILD file for STL containers and algorithms test

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

package(default_visibility = ["//tests:__pkg__", "//qemu:__pkg__"])

cc_binary(
    name = "stl_test",
    srcs = ["stl_test.cpp"],
    copts = ["-std=c++17"],
)
