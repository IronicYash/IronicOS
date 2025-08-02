<p align="center">
  <img src="https://img.shields.io/badge/IronicOS-Building-brightgreen?style=for-the-badge&logo=linux" alt="IronicOS Badge">
  <img src="https://img.shields.io/github/license/yourusername/IronicOS?style=for-the-badge" alt="License Badge">
</p>

<h1 align="center">🌀 IronicOS</h1>
<p align="center">
  A simple experimental operating system built from scratch with ❤️ in C, Assembly & GRUB. <br>
  Learn systems programming, kernel development, and bootloader basics—all in one project.
</p>

---

## 🚀 Getting Started

### 📦 Requirements
- GCC cross-compiler (`i686-elf-gcc`)
- `nasm` assembler
- `grub-pc-bin`, `xorriso`, `mtools`
- `make`, `qemu` for running the OS

### 🛠️ Build & Run

```bash
# Clone the repo
git clone https://github.com/IronicYash/IronicOS.git
cd IronicOS

# Build the kernel and ISO
make

# Run it in QEMU
make run

## 📂 Project Structure

```bash
.
├── boot/           # Bootloader (boot.asm)
├── build/          # Compiled object files and binaries
├── kernel/         # C source code for kernel
├── isodir/         # ISO layout with GRUB bootloader
├── linker.ld       # Linker script
├── grub.cfg        # GRUB menu configuration
├── Makefile        # Automated build process
└── IronicOS.iso    # Output bootable ISO
