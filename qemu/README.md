# QNX QEMU Runner

Run a QNX virtual machine (x86_64 or aarch64) with your cross-compiled binary baked into the filesystem.

## Architecture

The QEMU management scripts are implemented in Python using C++ design patterns.

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
# x86_64 target
sudo apt install qemu-system-x86

# aarch64 target
sudo apt install qemu-system-arm
```

### 2. Enable KVM acceleration (same-arch only)

KVM is used automatically when host and target architectures match and
`/dev/kvm` is writable. For cross-architecture emulation (e.g. aarch64
on an x86_64 host), QEMU falls back to software emulation — slower but
fully functional.

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
# x86_64
bazel run //qemu:run_qemu --config=qnx_x86_64

# aarch64
bazel run //qemu:run_qemu --config=qnx_aarch64
```

> **Important:** Run this in a regular terminal (not the VS Code chat terminal),
> otherwise closing the chat will kill the QEMU process.

> **To exit QEMU:** Press `Ctrl-A` then `X` in the terminal window. Typing `exit` only restarts the QNX shell.

This will:
1. Cross-compile `//src:hello_world` for the target architecture
2. Auto-discover and stage all test binaries from `//tests/...`
3. Build a QNX QEMU disk image with the binary at `/data/home/root/hello_world`
4. Launch QEMU with user-mode networking (SSH on `localhost:2222`)

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
| `QNX_QEMU_ARCH`      | `x86_64`   | Target architecture (`x86_64` or `aarch64`) |
| `QNX_QEMU_RAM`        | `1G`       | QEMU memory                       |
| `QNX_QEMU_SMP`        | `2`        | Number of CPU cores                |
| `QNX_ROOT`            | `~/qnx800` | QNX SDP install location          |

Example with custom settings:

```bash
QNX_QEMU_RAM=2G QNX_QEMU_SMP=4 bazel run //qemu:run_qemu --config=qnx_x86_64
```

## How it works

![QNX Filesystem Build Flow](../doc/qemu/qnx_filesystem_build.png)

1. `mkqnximage` (QNX SDP) builds an IFS boot image + VMDK disk image
2. Cross-compiled binaries are injected via `local/snippets/data_files.custom`
3. QEMU boots the IFS as `-kernel`, mounts the VMDK as the root disk
4. User-mode networking forwards SSH (host `localhost:2222` → guest `:22`)
5. KVM acceleration is used automatically when host arch matches target

VM workspace is stored in `.qnx_qemu_vm/` (git-ignored).

For full details on QNX image building, QEMU virtual devices, machine types,
and acceleration flags, see [README_QNX_QEMU_HW.md](README_QNX_QEMU_HW.md).