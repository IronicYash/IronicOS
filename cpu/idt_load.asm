; cpu/idt_load.asm  (NASM syntax)
global idt_load
extern idtp      ; declare idtp comes from C code / another object

section .text
idt_load:
    ; idtp is a 6-byte structure in C: 2-byte limit, 4-byte base
    lidt [idtp]
    ret
