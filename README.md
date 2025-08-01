# 🌀 IronicOS

**IronicOS** is a minimalist operating system written from scratch in **C** and **x86 Assembly**, built forlearning, and pure irony 

---

## 🛠 Features (Planned)

- [x] Bootloader (Assembly)
- [x] GRUB Multiboot compatible
- [x] C-based kernel entry
- [ ] VGA screen output
- [ ] Keyboard input
- [ ] Memory management
- [ ] Shell (basic command-line)
- [ ] File system
- [ ] Multitasking

---

## 📁 Project Structure
IronicOS/
├── boot/                  # Bootloader code
│   └── boot.asm
│
├── kernel/                # Kernel written in C
│   └── kernel.c
│
├── lib/                   # (Optional) Future low-level helper functions
│
├── include/               # (Optional) Shared header files
│
├── build/                 # Auto-generated compiled files (ignored by Git)
│   ├── IronicOS.iso       # Bootable ISO image (make output)
│   └── kernel.elf         # Compiled ELF binary (make output)
│
├── .github/               # GitHub Actions CI
│   └── workflows/
│       └── build.yml
│
├── .gitignore             # Ignore compiled & temporary files
├── linker.ld              # Linker script for memory layout
├── Makefile               # Build system
└── README.md              # Project overview

## License

IronicOS is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html).

You are free to use, modify, and distribute this software under the terms of the GPLv3.  
See the [LICENSE](./LICENSE) file for full details.
