[BITS 32]
global idt_load

section .text
; void idt_load(void* idtp);
idt_load:
    cli
    lidt [esp + 4]   ; Load IDT pointer from first argument on stack
    sti
    ret