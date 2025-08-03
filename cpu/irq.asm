[BITS 32]

; Export IRQ labels to C
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

; Reference to external IRQ handler (written in C)
EXTERN irq_handler

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
    add esp, 8         ; Pop IRQ number + dummy error code
    iret
%endmacro

; Define an IRQ handler stub
%macro DEFINE_IRQ 1
irq%1:
    cli
    push 0              ; Dummy error code
    push %1             ; IRQ number
    SAVE_REGS
    call irq_handler
    RESTORE_REGS
%endmacro

; Define all 16 IRQs
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
