# BUILD file for shared/dynamic library test
# Tests: cc_library with linkshared / cc_shared_library, or
#        simply building a cc_library that exercises the dynamic linking path.

# Note: QNX GCC supports building shared objects via -shared.
# We test both a regular library and a binary that links it.

cc_library(
    name = "stringutils",
    srcs = ["stringutils.cpp"],
    hdrs = ["stringutils.h"],
    copts = ["-std=c++17"],
)

# Build as a shared object (.so) to test dynamic library production
cc_binary(
    name = "libstringutils.so",
    srcs = ["stringutils.cpp", "stringutils.h"],
    copts = ["-std=c++17", "-fPIC"],
    linkopts = ["-shared"],
    linkshared = True,
)

cc_binary(
    name = "shared_lib_test",
    srcs = ["shared_lib_test.cpp"],
    copts = ["-std=c++17"],
    deps = [":stringutils"],
)
