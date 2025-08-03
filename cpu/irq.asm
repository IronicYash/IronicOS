[BITS 32]
GLOBAL irq0
GLOBAL irq1
GLOBAL irq2
GLOBAL irq3
GLOBAL irq4
GLOBAL irq5
GLOBAL irq6
GLOBAL irq7
GLOBAL irq8
GLOBAL irq9
GLOBAL irq10
GLOBAL irq11
GLOBAL irq12
GLOBAL irq13
GLOBAL irq14
GLOBAL irq15
[EXTERN irq_handler]

; Save CPU state
%macro SAVE_REGS 0
    pusha
    push ds
    push es
    push fs
    push gs
%endmacro

; Restore CPU state
%macro RESTORE_REGS 0
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 4     ; Pop error code placeholder (0 for IRQs)
    iret
%endmacro

; General IRQ stub
%macro DEFINE_IRQ 1
irq%1:
    push 0              ; Push dummy error code
    push %1             ; Push IRQ number
    SAVE_REGS
    call irq_handler
    RESTORE_REGS
%endmacro

DEFINE_IRQ 0
DEFINE_IRQ 1
DEFINE_IRQ 2
DEFINE_IRQ 3
DEFINE_IRQ 4
DEFINE_IRQ 5
DEFINE_IRQ 6
DEFINE_IRQ 7
DEFINE_IRQ 8
DEFINE_IRQ 9
DEFINE_IRQ 10
DEFINE_IRQ 11
DEFINE_IRQ 12
DEFINE_IRQ 13
DEFINE_IRQ 14
DEFINE_IRQ 15