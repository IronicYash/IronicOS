; kernel/multiboot_header.asm
; Multiboot v1 header (explicit constants + correct checksum)
section .multiboot
    align 4

    MB_MAGIC    equ 0x1BADB002
    MB_FLAGS    equ 0x0            ; change if you need memory map or modules
    MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

    dd MB_MAGIC        ; magic
    dd MB_FLAGS        ; flags
    dd MB_CHECKSUM     ; checksum (magic + flags + checksum == 0)
