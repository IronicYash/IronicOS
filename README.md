# 🌀 IronicOS

**IronicOS** is a minimalist hobby operating system written from scratch in **C** and **x86 Assembly**, built for fun, learning, and pure irony — because why not write an OS in 2025?

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