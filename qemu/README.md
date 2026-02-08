# QNX QEMU Runner

Run a QNX x86_64 virtual machine with your cross-compiled binary baked into the filesystem.

## Architecture

The QEMU management scripts are implemented in Python using C++ design patterns:

```
qemu/
├── run_qnx_qemu.py          # Entry point: build image & launch QEMU
├── stop_qnx_qemu.py         # Entry point: stop running VM
├── clean_qnx_qemu.py        # Entry point: wipe VM workspace
└── qnx_qemu/                # Python package (domain library)
    ├── interfaces.py         # ABCs (C++ pure-virtual interfaces)
    ├── config.py             # QemuConfig dataclass (env var defaults)
    ├── logger.py             # Structured console output
    ├── path_resolver.py      # Workspace / binary / SDK path resolution
    ├── ssh.py                # SSH key detection / generation
    ├── vm_workspace.py       # VM directory & staging management
    ├── process.py            # PID-file-based process lifecycle
    ├── acceleration.py       # Strategy pattern: KVM vs emulation
    ├── qemu_command.py       # Builder pattern: QEMU CLI construction
    ├── image_builder.py      # mkqnximage wrapper (IImageBuilder)
    └── manager.py            # Facade: QemuManager orchestrates all
```

### Design Patterns Used

| Pattern      | Where                          | Purpose                                           |
|--------------|--------------------------------|---------------------------------------------------|
| **Interface**| `interfaces.py`                | ABC contracts for every service (like C++ pure virtual) |
| **Strategy** | `acceleration.py`              | KVM vs software emulation, swappable at runtime   |
| **Builder**  | `qemu_command.py`              | Fluent construction of the QEMU argument list     |
| **Facade**   | `manager.py`                   | Single `QemuManager` entry point for run/stop/clean |
| **DI**       | `QemuManager.__init__`         | Optional injection of every dependency interface  |

## Prerequisites (one-time setup)

### 1. Install QEMU

```bash
sudo apt install qemu-system-x86
```

### 2. Enable KVM acceleration

```bash
sudo adduser $(whoami) kvm
```

Then **close and reopen your WSL2 terminal** for the group to take effect.
(Or run `newgrp kvm` in the current session as a one-off.)

### 3. SSH key

The script auto-generates `~/.ssh/id_ed25519` if you don't have one.
If you already have a key, it will be used automatically.

## Usage

### Launch the QNX VM

```bash
bazel run //qemu:run_qemu --config=qnx_x86_64
```

> **Important:** Run this in a regular terminal (not the VS Code chat terminal),
> otherwise closing the chat will kill the QEMU process.

> **To exit QEMU:** Press `Ctrl-A` then `X` in the terminal window. Typing `exit` only restarts the QNX shell.

This will:
1. Cross-compile `//src:hello_world` for QNX x86_64
2. Build a QNX QEMU disk image with the binary at `/data/home/root/hello_world`
3. Launch QEMU with user-mode networking (SSH on `localhost:2222`)

### SSH into the running VM

From a **second terminal**:

```bash
ssh -o StrictHostKeyChecking=no -p 2222 root@localhost
```

### Run the demo binary

Inside the QNX shell:

```bash
/data/home/root/hello_world
```

### Exit QEMU

Press **Ctrl-A** then **X** in the QEMU console.

### Stop / Clean

```bash
# Stop a running VM
bazel run //qemu:stop_qemu

# Wipe the VM directory entirely (.qnx_qemu_vm/)
bazel run //qemu:clean_qemu
```

## Configuration

| Environment Variable   | Default    | Description                        |
|------------------------|------------|------------------------------------|
| `QNX_QEMU_SSH_PORT`   | `2222`     | Host port forwarded to guest SSH   |
| `QNX_QEMU_RAM`        | `1G`       | QEMU memory                       |
| `QNX_QEMU_SMP`        | `2`        | Number of CPU cores                |
| `QNX_ROOT`            | `~/qnx800` | QNX SDP install location          |

Example with custom settings:

```bash
QNX_QEMU_RAM=2G QNX_QEMU_SMP=4 bazel run //qemu:run_qemu --config=qnx_x86_64
```

## How it works

- Uses `mkqnximage` from the QNX SDP to build the IFS + disk image
- Injects the Bazel-built binary via `local/snippets/data_files.custom`
- Launches QEMU with **user-mode networking** (no bridge/libvirt/root needed)
- KVM acceleration is used automatically when `/dev/kvm` is writable
- VM workspace is stored in `.qnx_qemu_vm/` (git-ignored)