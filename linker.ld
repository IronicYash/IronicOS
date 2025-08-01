ENTRY(start)

SECTIONS
{
  /* Load the kernel at 1MB (GRUB recommended) */
  . = 1M;

  /* Text section (code) */
  .text :
  {
    *(.multiboot)
    *(.text)
  }

  /* Data section (initialized data) */
  .data :
  {
    *(.data)
  }

  /* BSS section (uninitialized data) */
  .bss :
  {
    *(COMMON)
    *(.bss)
  }
}