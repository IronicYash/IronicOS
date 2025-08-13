; kernel/entry.asm
; Production-ready kernel entry for i386 (NASM syntax)
; - Sets up a safe stack
; - Sets DS/ES/FS/GS to a sane data selector (0x10)
; - Calls kernel_main (C)
; - Halts if kernel_main returns

global _start
extern kernel_main

section .text
_start:
    cli                 ; disable interrupts while we setup stack/segments
    cld                 ; clear direction flag (string ops go forward)

    ; stack_top is defined in the BSS below; set ESP to its address
    mov eax, stack_top
    mov esp, eax
    and esp, 0xFFFFFFF0 ; optional: align stack to 16 bytes for best practice
    ; Assumes a flat GDT where 0x10 is the kernel data selector.
    ; If you use a custom GDT, change the selector accordingly.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; -------------------------
    ; Call into C kernel
    ; -------------------------
    call kernel_main

    ; If kernel_main returns, halt the CPU in a safe loop
.halt_loop:
    cli
    hlt
    jmp .halt_loop

section .bss
align 16
stack_bottom:
    ; Reserve 16 KiB for the kernel stack (adjust if needed)
    resb 16384
stack_top: