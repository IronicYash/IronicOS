; cpu/irq.asm — matches irq.c (irq_dispatch, irq_remap to 32..47)
[BITS 32]

; We compute IDT vector as 32 + IRQ#
%macro IRQ_STUB 1
    global irq%1
irq%1:
    cli
    push dword 0           ; err_code (dummy)
    push dword (32+%1)     ; int_no (IDT index after remap)
    jmp irq_common_stub
%endmacro

extern irq_dispatch

irq_common_stub:
    pusha
    push ds
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax
    call irq_dispatch      ; C: void irq_dispatch(struct registers* r)
    add esp, 4

    pop ds
    popa
    add esp, 8             ; drop [int_no, err_code]
    sti
    iretd

; IRQs 0..15 → IDT 32..47
IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15