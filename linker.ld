ENTRY(_start)

SECTIONS {
  . = 1M;

  /* Multiboot2 header must be within the first 32 KiB */
  .multiboot_header ALIGN(8) : {
    KEEP(*(.multiboot_header))
  }

  .text ALIGN(4K) : {
    *(.text*)
  }

  .rodata ALIGN(4K) : {
    *(.rodata*)
  }

  .data ALIGN(4K) : {
    *(.data*)
  }

  .bss ALIGN(4K) : {
    *(COMMON)
    *(.bss*)
  }
}
