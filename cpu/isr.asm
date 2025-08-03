[BITS 32]

[EXTERN isr_handler]

; --------- Macros to define ISRs ---------

%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push 0              ; Push dummy error code
    push %1             ; Push interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push %1             ; Push interrupt number only (error code already on stack)
    jmp isr_common_stub
%endmacro

; --------- ISRs without error code ---------
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_NOERR 9
ISR_NOERR 10
ISR_NOERR 11
ISR_NOERR 12
ISR_NOERR 13
ISR_NOERR 14
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; --------- ISRs with error code ---------
ISR_ERR 8
ISR_ERR 15

; --------- Common stub for all ISRs ---------
global isr_common_stub
isr_common_stub:
    pusha                      ; Push all general-purpose registers
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10               ; Kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                   ; Pass pointer to struct to C handler
    call isr_handler
    add esp, 4                 ; Clean up stack (esp pushed above)

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8                 ; Remove int number and error code
    sti
    iret