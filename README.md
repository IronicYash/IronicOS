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
├IronicOS/
├── Makefile
├── linker.ld
├── grub.cfg

├── build/                   # <== Compiled .o and kernel.elf go here
│   └── (auto-generated)

├── isodir/                  # <== ISO build directory
│   └── boot/
│       ├── grub/
│       │   └── grub.cfg
│       └── kernel.elf

├── kernel/
│   ├── entry.asm
│   ├── multiboot_header.asm
│   └── kernel_main.c

├── cpu/
│   ├── idt.c
│   ├── idt.h
│   ├── irq.c
│   ├── irq.h
│   ├── irq.asm
│   ├── isr.c
│   ├── isr.h
│   └── isr.asm

├── lib/
│   ├── screen.c
│   ├── screen.h
│   ├── keyboard.c
│   ├── keyboard.h
│   ├── timer.c
│   ├── timer.h
│   └── ports.h              # <== Required for I/O port access

