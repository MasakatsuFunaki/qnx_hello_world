# BUILD file for QNX-specific API tests
# These tests use QNX headers (neutrino, syspage, etc.) and verify the
# toolchain correctly locates QNX SDP headers and libraries.

package(default_visibility = ["//tests:__pkg__", "//qemu:__pkg__"])

cc_binary(
    name = "qnx_api_test",
    srcs = ["qnx_api_test.cpp"],
    copts = ["-std=c++17"],
    linkopts = ["-lsocket"],
)

cc_binary(
    name = "timer_pulse_test",
    srcs = ["timer_pulse_test.cpp"],
    copts = ["-std=c++17"],
)
