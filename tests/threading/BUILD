# BUILD file for threading / concurrency test

cc_binary(
    name = "threading_test",
    srcs = ["threading_test.cpp"],
    copts = ["-std=c++17"],
    # QNX includes pthreads in libc, no separate -lpthread needed
)
