[BITS 32]

[GLOBAL _start]           ; Entry point symbol expected by linker
[EXTERN kernel_main]      ; C kernel entry point

section .text
_start:
    cli                   ; Disable interrupts while setting up

    ; Set up stack pointer
    mov esp, stack_top

    ; Call into the C kernel
    call kernel_main

.hang:
    hlt                   ; Halt if kernel_main ever returns
    jmp .hang

section .bss
align 4
resb 4096                 ; 4 KB kernel stack
stack_bottom:
stack_top: