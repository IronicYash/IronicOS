ENTRY(_start)

SECTIONS
{
    /* Kernel loads at 1MB */
    . = 1M;

    .text : {
        *(.multiboot)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
    }
}
