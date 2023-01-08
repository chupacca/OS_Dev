### Table of Contents
> (1) Build Files
> (2) Build Commands
> (3) Nightly Stuff
> (4) Rustup
> (5) Emulator: QEMU

# (1) Build Files
* `~/.cargo/config.toml`
```toml
# in .cargo/config.toml

[unstable]
build-std-features = ["compiler-builtins-mem"]
build-std = ["core", "compiler_builtins"]
```

* `Cargo.toml`
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

* `rust-toolchain.toml`
```toml
[toolchain]
channel="nightly"
```

---

# (2) Build Commands

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

---

# (3) **Nightly Stuff**

**In project directory: `Cargo.toml `**
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

**In project directory: `rust-toolchain.toml`**
```toml
[toolchain]
channel="nightly"
```

**The bash build command using `cargo` with nightly:**
```bash
cargo +nightly [CMDS]
```

**Other Possible Build Commands:**
```bash
cargo build --target x86_64-rust_os.json
```
```shell
cargo install bootimage --target x86_64-unknown-none
```
```bash
cargo build --target x86_64-unknown-linux-gnu
```

---

# (4) Rustup

**List rust components**
```bash
rustup component list
```

---

# (5) Emulator: QEMU

**Installing QEMU:**
```bash
sudo apt install qemu-user qemu-user-static gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu binutils-aarch64-linux-gnu-dbg build-essential
```

**Running with QEMU:**
```bash
qemu-system-x86_64 -drive format=raw,file=target/x86_64-rust_os/debug/bootimage-rust_os.bin
```

---

