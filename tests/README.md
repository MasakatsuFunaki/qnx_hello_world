# QNX Bazel Toolchain Test Suite

This directory contains comprehensive tests for the QNX cross-compilation
Bazel toolchain. The tests verify that the toolchain correctly handles
a wide range of C++ features, library types, and build configurations.

## Test Categories

| Directory | What it tests |
|-----------|---------------|
| `cpp_features/` | Modern C++ language features (C++11/14/17) |
| `stl_containers/` | STL containers and algorithms |
| `threading/` | std::thread, mutex, atomics, condition_variable |
| `lib_static/` | Building and linking static libraries |
| `lib_shared/` | Building and linking shared/dynamic libraries |
| `lib_header_only/` | Header-only library consumption |
| `lib_chain/` | Multi-level library dependency chains |
| `build_features/` | copts, defines, select(), includes |
| `qnx_specific/` | QNX-specific POSIX and system APIs |

## Running All Compile Tests

```bash
# Build everything for aarch64
bazel build //tests/... --config=qnx_aarch64

# Build everything for x86_64
bazel build //tests/... --config=qnx_x86_64
```

## Running Individual Test Groups

```bash
bazel build //tests/cpp_features:all --config=qnx_aarch64
bazel build //tests/threading:all --config=qnx_aarch64
bazel build //tests/lib_chain:all --config=qnx_aarch64
```
