; cpu/isr_irq_stubs.asm  (NASM - explicit stubs)
; 32 exception ISRs and 16 IRQ stubs + isr_stub_table and irq_stub_table
; Assemble: nasm -f elf32 cpu/isr_irq_stubs.asm -o build/cpu/isr_irq_stubs_asm.o

BITS 32

global isr_stub_table
global irq_stub_table

extern isr_handler_c    ; C: void isr_handler_c(uint32_t vec)
extern irq_handler_c    ; C: void irq_handler_c(uint32_t vec)

section .text

; ------------------ ISRs 0..31 ------------------
global isr_stub_0
isr_stub_0:
    push dword 0
    jmp isr_common_stub

global isr_stub_1
isr_stub_1:
    push dword 1
    jmp isr_common_stub

global isr_stub_2
isr_stub_2:
    push dword 2
    jmp isr_common_stub

global isr_stub_3
isr_stub_3:
    push dword 3
    jmp isr_common_stub

global isr_stub_4
isr_stub_4:
    push dword 4
    jmp isr_common_stub

global isr_stub_5
isr_stub_5:
    push dword 5
    jmp isr_common_stub

global isr_stub_6
isr_stub_6:
    push dword 6
    jmp isr_common_stub

global isr_stub_7
isr_stub_7:
    push dword 7
    jmp isr_common_stub

global isr_stub_8
isr_stub_8:
    push dword 8
    jmp isr_common_stub

global isr_stub_9
isr_stub_9:
    push dword 9
    jmp isr_common_stub

global isr_stub_10
isr_stub_10:
    push dword 10
    jmp isr_common_stub

global isr_stub_11
isr_stub_11:
    push dword 11
    jmp isr_common_stub

global isr_stub_12
isr_stub_12:
    push dword 12
    jmp isr_common_stub

global isr_stub_13
isr_stub_13:
    push dword 13
    jmp isr_common_stub

global isr_stub_14
isr_stub_14:
    push dword 14
    jmp isr_common_stub

global isr_stub_15
isr_stub_15:
    push dword 15
    jmp isr_common_stub

global isr_stub_16
isr_stub_16:
    push dword 16
    jmp isr_common_stub

global isr_stub_17
isr_stub_17:
    push dword 17
    jmp isr_common_stub

global isr_stub_18
isr_stub_18:
    push dword 18
    jmp isr_common_stub

global isr_stub_19
isr_stub_19:
    push dword 19
    jmp isr_common_stub

global isr_stub_20
isr_stub_20:
    push dword 20
    jmp isr_common_stub

global isr_stub_21
isr_stub_21:
    push dword 21
    jmp isr_common_stub

global isr_stub_22
isr_stub_22:
    push dword 22
    jmp isr_common_stub

global isr_stub_23
isr_stub_23:
    push dword 23
    jmp isr_common_stub

global isr_stub_24
isr_stub_24:
    push dword 24
    jmp isr_common_stub

global isr_stub_25
isr_stub_25:
    push dword 25
    jmp isr_common_stub

global isr_stub_26
isr_stub_26:
    push dword 26
    jmp isr_common_stub

global isr_stub_27
isr_stub_27:
    push dword 27
    jmp isr_common_stub

global isr_stub_28
isr_stub_28:
    push dword 28
    jmp isr_common_stub

global isr_stub_29
isr_stub_29:
    push dword 29
    jmp isr_common_stub

global isr_stub_30
isr_stub_30:
    push dword 30
    jmp isr_common_stub

global isr_stub_31
isr_stub_31:
    push dword 31
    jmp isr_common_stub

; ------------------ IRQs 0..15 (vectors 32..47) ------------------
global irq_stub_0
irq_stub_0:
    push dword 32
    jmp irq_common_stub

global irq_stub_1
irq_stub_1:
    push dword 33
    jmp irq_common_stub

global irq_stub_2
irq_stub_2:
    push dword 34
    jmp irq_common_stub

global irq_stub_3
irq_stub_3:
    push dword 35
    jmp irq_common_stub

global irq_stub_4
irq_stub_4:
    push dword 36
    jmp irq_common_stub

global irq_stub_5
irq_stub_5:
    push dword 37
    jmp irq_common_stub

global irq_stub_6
irq_stub_6:
    push dword 38
    jmp irq_common_stub

global irq_stub_7
irq_stub_7:
    push dword 39
    jmp irq_common_stub

global irq_stub_8
irq_stub_8:
    push dword 40
    jmp irq_common_stub

global irq_stub_9
irq_stub_9:
    push dword 41
    jmp irq_common_stub

global irq_stub_10
irq_stub_10:
    push dword 42
    jmp irq_common_stub

global irq_stub_11
irq_stub_11:
    push dword 43
    jmp irq_common_stub

global irq_stub_12
irq_stub_12:
    push dword 44
    jmp irq_common_stub

global irq_stub_13
irq_stub_13:
    push dword 45
    jmp irq_common_stub

global irq_stub_14
irq_stub_14:
    push dword 46
    jmp irq_common_stub

global irq_stub_15
irq_stub_15:
    push dword 47
    jmp irq_common_stub

; ------------------ common handlers ------------------
isr_common_stub:
    cli
    pusha
    push ds
    push es
    mov ax, 0x10       ; kernel data selector (adjust if your GDT differs)
    mov ds, ax
    mov es, ax
    call isr_handler_c
    pop es
    pop ds
    popa
    add esp, 4         ; remove pushed vector
    sti
    iret

irq_common_stub:
    cli
    pusha
    push ds
    push es
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    call irq_handler_c
    pop es
    pop ds
    popa
    add esp, 4
    ; EOI to PICs: slave then master
    mov al, 0x20
    out 0xA0, al
    out 0x20, al
    sti
    iret

section .data
; Table of pointers to each stub for the C code to use
isr_stub_table:
    dd isr_stub_0
    dd isr_stub_1
    dd isr_stub_2
    dd isr_stub_3
    dd isr_stub_4
    dd isr_stub_5
    dd isr_stub_6
    dd isr_stub_7
    dd isr_stub_8
    dd isr_stub_9
    dd isr_stub_10
    dd isr_stub_11
    dd isr_stub_12
    dd isr_stub_13
    dd isr_stub_14
    dd isr_stub_15
    dd isr_stub_16
    dd isr_stub_17
    dd isr_stub_18
    dd isr_stub_19
    dd isr_stub_20
    dd isr_stub_21
    dd isr_stub_22
    dd isr_stub_23
    dd isr_stub_24
    dd isr_stub_25
    dd isr_stub_26
    dd isr_stub_27
    dd isr_stub_28
    dd isr_stub_29
    dd isr_stub_30
    dd isr_stub_31

irq_stub_table:
    dd irq_stub_0
    dd irq_stub_1
    dd irq_stub_2
    dd irq_stub_3
    dd irq_stub_4
    dd irq_stub_5
    dd irq_stub_6
    dd irq_stub_7
    dd irq_stub_8
    dd irq_stub_9
    dd irq_stub_10
    dd irq_stub_11
    dd irq_stub_12
    dd irq_stub_13
    dd irq_stub_14
    dd irq_stub_15
