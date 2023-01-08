### How to Build and Emulate this code:
**Go into `code` directory:**
```bash
cd os_dev/rust_os/code
```
**Build command(for x86_64):**
```bash
cargo +nightly build --target x86_64-rust_os.json
```
**Build BootImage(for x86_64):**
```bash
cargo +nightly bootimage --target x86_64-rust_os.json
```
**Run in emulator(for x86_64):**
```bash
qemu-system-x86_64 -drive format=raw,file=target/x86_64-rust_os/debug/bootimage-rust_os.bin
```
