; cpu/idt_flush.asm
[BITS 32]

global idt_flush

idt_flush:
    mov eax, [esp + 4]   ; Get the pointer to the IDT (argument)
    lidt [eax]           ; Load it into the IDTR
    ret
