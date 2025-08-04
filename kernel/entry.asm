global _start
extern kernel_main

section .text
_start:
    cli                     ; Disable interrupts
    call kernel_main        ; Call main kernel code
    hlt                     ; Halt CPU