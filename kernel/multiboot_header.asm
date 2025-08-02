section .multiboot_header
align 8

multiboot2_header:
    dd 0xE85250D6            ; Magic
    dd 0                     ; Architecture (0 = i386)
    dd header_end - multiboot2_header ; Header length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - multiboot2_header)) ; Checksum

    ; --- Required End Tag ---
    dw 0                     ; Tag Type (0 = End)
    dw 0                     ; Flags
    dd 8                     ; Size of tag

header_end: