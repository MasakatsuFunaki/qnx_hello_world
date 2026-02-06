# BUILD file for static library test
# Tests: cc_library (static), deps, hdrs, includes

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
