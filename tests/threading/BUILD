# BUILD file for threading / concurrency test

package(default_visibility = ["//tests:__pkg__", "//qemu:__pkg__"])

cc_binary(
    name = "threading_test",
    srcs = ["threading_test.cpp"],
    copts = ["-std=c++17"],
    # QNX includes pthreads in libc, no separate -lpthread needed
)
