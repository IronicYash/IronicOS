/* linker.ld - production-ready for IronicOS */
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS
{
    /* Load the kernel at 1 MiB */
    . = 1M;

    /* Multiboot header */
    .multiboot ALIGN(4) : {
        KEEP(*(.multiboot))
    }

    /* Code section */
    .text ALIGN(4K) : {
        *(.text.boot)
        *(.text*)
    }

    /* Read-only data */
    .rodata ALIGN(4K) : {
        *(.rodata*)
    }

    /* Initialized data */
    .data ALIGN(4K) : {
        *(.data*)
    }

    /* BSS + kernel stack */
    .bss ALIGN(16) : {
        bss_start = .;
        *(COMMON)
        *(.bss*)
        . = ALIGN(16);

        /* Reserve kernel stack */
        __stack_bottom = .;
        . += 64K;        /* 64 KiB stack */
        __stack_top = .;

        bss_end = .;
    }

    /* Aliases for entry.asm */
    stack_bottom = __stack_bottom;
    stack_top    = __stack_top;
}
