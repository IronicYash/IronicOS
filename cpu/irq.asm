; irq.asm - Hardware IRQ stubs (32-47)

[bits 32]
[extern irq_handler]

%macro IRQ 1
global irq%1
irq%1:
    cli
    push dword 0         ; IRQs don’t push error code
    push dword (32 + %1)
    jmp common_irq_stub
%endmacro

section .text

common_irq_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8           ; Clean error code and int number
    sti
    iret

; Define all 16 IRQs
IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

global irq_stub_table
irq_stub_table:
    dd irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
    dd irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15