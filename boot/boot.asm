; boot/boot.asm - bootloader entry
BITS 16
ORG 0x7c00

start:
    mov ah, 0x0e       ; BIOS teletype function
    mov al, 'I'
    int 0x10

    mov al, 'R'
    int 0x10

    mov al, 'O'
    int 0x10

    cli
    hlt

times 510 - ($ - $$) db 0
dw 0xAA55
