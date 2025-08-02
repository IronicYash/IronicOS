; filepath: kernel/entry.asm
BITS 32

global _start
extern kernel_main

section .text
_start:
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang