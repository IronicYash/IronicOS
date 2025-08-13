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

.IronicOS/
├── Makefile                # Main build file (supports make, make iso, make run, make clean)
├── linker.ld                # Linker script for kernel
├── .gitignore               # Ignore build artifacts & temp files
├── .gitattributes           # GitHub syntax highlighting & LF line ending rules
├── .editorconfig            # Editor formatting rules
├── grub/
│   └── grub.cfg             # GRUB boot menu configuration (copied into ISO at build time)
├── build/                   # (Generated) Compiled object files and kernel.elf
├── isodir/                  # (Generated) ISO root directory
│   └── boot/
│       ├── kernel.elf       # Compiled kernel binary
│       └── grub/
│           └── grub.cfg     # GRUB config inside ISO
├── kernel/
│   ├── kernel_main.c        # Kernel entry point (after assembly boot)
│   ├── entry.asm            # Early assembly entry point (sets up stack, jumps to C)
│   └── multiboot_header.asm # Multiboot header for GRUB
├── cpu/
│   ├── idt.c / idt.h        # Interrupt Descriptor Table setup
│   ├── idt_load.asm         # Assembly helper to load IDT
│   ├── isr.c / isr.h        # Interrupt Service Routines (CPU exceptions)
│   ├── isr.asm              # ISR stubs in assembly
│   ├── irq.c / irq.h        # IRQ handlers (hardware interrupts)
│   └── irq.asm              # IRQ stubs in assembly
├── lib/
│   ├── screen.c / screen.h  # VGA text mode output
│   ├── keyboard.c / keyboard.h # PS/2 keyboard input handling
│   ├── string.c / string.h  # Basic string/memory functions
│   ├── timer.c / timer.h    # PIT timer handling
│   └── shell.c / shell.h    # Simple kernel shell
└── memory/                  # (Future) Memory management code