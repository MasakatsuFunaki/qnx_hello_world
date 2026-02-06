# BUILD file for header-only library test
# Tests: cc_library with hdrs only (no srcs), deps on header-only lib

cc_library(
    name = "event",
    hdrs = ["event.hpp"],
    copts = ["-std=c++17"],
    # Header-only: no srcs
)

cc_binary(
    name = "header_only_test",
    srcs = ["header_only_test.cpp"],
    copts = ["-std=c++17"],
    deps = [":event"],
)
