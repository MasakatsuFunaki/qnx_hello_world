# BUILD file for multi-level library dependency chain test
# Tests: transitive deps, cc_library depending on another cc_library
#
# Dependency graph:   app -> config -> logger

load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//tests:__pkg__", "//qemu:__pkg__"])

cc_library(
    name = "logger",
    srcs = ["logger.cpp"],
    hdrs = ["logger.h"],
    copts = ["-std=c++17"],
)

cc_library(
    name = "config",
    srcs = ["config.cpp"],
    hdrs = ["config.h"],
    copts = ["-std=c++17"],
    deps = [":logger"],
)

cc_library(
    name = "app",
    srcs = ["app.cpp"],
    hdrs = ["app.h"],
    copts = ["-std=c++17"],
    deps = [
        ":config",
        ":logger",
    ],
)

cc_binary(
    name = "chain_test",
    srcs = ["chain_test.cpp"],
    copts = ["-std=c++17"],
    deps = [":app"],
)
