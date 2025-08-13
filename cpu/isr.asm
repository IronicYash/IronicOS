; cpu/isr.asm — matches isr.c (isr_dispatch, register_isr_handler, etc.)
[BITS 32]

; --- Macros: set up top-of-stack as [int_no, err_code] before common stub ---

%macro ISR_NOERRCODE 1
    global isr%1
isr%1:
    cli
    push dword 0           ; err_code (dummy)
    push dword %1          ; int_no
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
isr%1:
    cli
    push dword %1          ; int_no
    ; CPU already pushed err_code (on top) before EIP/CS/EFLAGS
    jmp isr_common_stub
%endmacro

extern isr_dispatch

isr_common_stub:
    pusha                  ; save GPRs (EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI)
    push ds                ; save DS (your struct has 'ds'; we only store DS)
    mov ax, 0x10           ; kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp           ; pointer to registers struct (starts at saved DS)
    push eax
    call isr_dispatch      ; C: void isr_dispatch(struct registers* r)
    add esp, 4

    pop ds                 ; restore DS
    popa                   ; restore GPRs
    add esp, 8             ; drop [int_no, err_code] (dummy/CPU)
    sti
    iretd

; ---- CPU exceptions 0..31 (Intel) ----
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31