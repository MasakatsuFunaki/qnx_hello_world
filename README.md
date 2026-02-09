# QNX Hello World (Bazel)

Cross-compiled C++ hello world for QNX 8.0 using Bazel.

## What's Inside

- **[src/](src/README.md)** — Demo binary, cross-compiled for QNX x86_64 and aarch64
- **[tests/](tests/README.md)** — C++ test suite: modern C++, STL, threading, libraries, build configs, QNX APIs
- **[qemu/](qemu/README.md)** — Builds a QNX QEMU disk image with binaries baked in, launches the VM
- **[qemu/ technical](qemu/README_QNX_QEMU_HW.md)** — mkqnximage, IFS/VMDK images, QEMU virtual devices, KVM acceleration
- **[toolchain/](toolchain/README.md)** — Bazel cross-compilation toolchain for QNX SDP 8.0

## Quick Start

```bash
# Build
bazel build  //... --config=qnx_x86_64

# Build on QNX (QEMU)

bazel build  //qemu:run_qemu --config=qnx_x86_64
bazel build //qemu:run_qemu --config=qnx_aarch64

# Run on QNX (QEMU)

bazel run //qemu:run_qemu --config=qnx_x86_64
bazel run //qemu:run_qemu --config=qnx_aarch64

#Stop and clean a  QEMU instance 
bazel run //qemu:clean_qemu
bazel run //qemu:stop_qemu
```

## License

This project is licensed under Apache License Version 2.0, January 2004
