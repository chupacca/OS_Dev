This is the `~/.cargo/config.toml`
```toml
# in .cargo/config.toml

[unstable]
build-std-features = ["compiler-builtins-mem"]
build-std = ["core", "compiler_builtins"]
```

**Build command:**
```bash
cargo +nightly build --target x86_64-rust_os.json
```
**Build BootImage:**
```bash
cargo +nightly bootimage --target x86_64-rust_os.json
```
**Run in emulator:**
```bash
qemu-system-x86_64 -drive format=raw,file=target/x86_64-rust_os/debug/bootimage-rust_os.bin
```

**Nightly Stuff**

In project directory: `Cargo.toml `
```toml
[toolchain]
channel = "nightly"

[package]
name = "rust_os"
version = "0.1.0"
edition = "2021"

[dependencies]
bootloader = "0.9.8"
```

In project directory: `rust-toolchain.toml`
```toml
[toolchain]
channel="nightly"
```

The bash build command using `cargo` with nightly:
```bash
cargo +nightly [CMDS]
```

---

List rust components
```bash
rustup component list
```

---

Installing QEMU:
```bash
sudo apt install qemu-user qemu-user-static gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu binutils-aarch64-linux-gnu-dbg build-essential
```

Build:
```bash
cargo build --target x86_64-rust_os.json
```
```shell
cargo install bootimage --target x86_64-unknown-none
```
```bash
cargo build --target x86_64-unknown-linux-gnu
```

Running with QEMU:
```bash
qemu-system-x86_64 -drive format=raw,file=target/x86_64-rust_os/debug/bootimage-rust_os.bin
```

---

