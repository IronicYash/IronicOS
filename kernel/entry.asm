; entry.asm - IronicOS bootstrap
; -------------------------------
BITS 32
GLOBAL _start

extern kernel_main       ; kernel main in C
extern bss_start         ; start of .bss
extern bss_end           ; end of .bss
extern stack_top         ; top of kernel stack

_start:
    ; -----------------------------
    ; Set up the stack
    ; -----------------------------
    mov esp, stack_top
    mov ebp, esp

    ; -----------------------------
    ; Zero out .bss
    ; -----------------------------
    mov edi, bss_start
    mov ecx, bss_end
    sub ecx, edi           ; ECX = size of BSS
    xor eax, eax           ; fill with 0
    shr ecx, 2             ; divide by 4 for 32-bit words
    jz .bss_done
    rep stosd              ; fill .bss with 0
.bss_done:

    ; -----------------------------
    ; Call kernel_main
    ; -----------------------------
    call kernel_main

    ; -----------------------------
    ; Halt CPU if kernel_main returns
    ; -----------------------------
.halt:
    cli
    hlt
    jmp .halt
