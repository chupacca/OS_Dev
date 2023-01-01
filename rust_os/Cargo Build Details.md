This is the `~/.cargo/config.toml`
```toml
# in .cargo/config.toml

[unstable]
build-std-features = ["compiler-builtins-mem"]
build-std = ["core", "compiler_builtins"]
```

Build command:
```bash
cargo build --target x86_64-rust_os.json
```
```bash
cargo build --target x86_64-unknown-linux-gnu
```

**Nightly Stuff**

Cargo.toml 
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

rust-toolchain.toml
```toml
[toolchain]
channel="nightly"
```

Cargo with nightly:
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

---

