# QNX Hello World (Bazel)

Cross-compiled C++ hello world for QNX 8.0 using Bazel.

## Supported Platforms

| Config | Target |
|---|---|
| `qnx_x86_64` | QNX on Intel/AMD 64-bit |
| `qnx_aarch64` | QNX on ARM 64-bit |

## Prerequisites

- Bazel 8+
- QNX SDP 8.0 installed at `$HOME/qnx800` (or set `QNX_ROOT` / source `qnxsdp-env.sh`)

## Build

```bash
# x86_64
bazel build //src:hello_world --config=qnx_x86_64

# aarch64 (ARM)
bazel build //src:hello_world --config=qnx_aarch64
```

Output binary: `bazel-bin/src/hello_world`

## Architecture

![Bazel Build Architecture](doc/bazel/bazel_architecture.png)

See [doc/bazel/bazel_architecture.puml](doc/bazel/bazel_architecture.puml) for the PlantUML source.

To regenerate the PNG:

```bash
cd doc/bazel
java -jar plantuml.jar -tpng bazel_architecture.puml
```

> Download `plantuml.jar` from [plantuml.com/download](https://plantuml.com/download). Requires Java 17+.

## Project Structure

```
├── src/
│   ├── BUILD              # cc_binary target
│   └── main.cpp           # Source code
├── toolchain/
│   ├── BUILD              # Platform & toolchain definitions
│   ├── cc_toolchain_config.bzl  # Cross-compiler config
│   └── qnx_sdk.bzl       # QNX SDP repo rule
├── doc/bazel/
│   └── bazel_architecture.puml  # Architecture diagram
├── MODULE.bazel           # Bazel module config
└── .bazelrc               # Build configs
```
