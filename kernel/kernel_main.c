#include "kernel.h"
#include "../lib/screen.h"
#include "../lib/keyboard.h"
#include "memory.h"
#include "../lib/string.h"
#include "../cpu/irq.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"


__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
    0xe85250d6,    // magic number (Multiboot2)
    0,             // architecture (0 = i386)
    0,             // header length (filled by linker)
    -(0xe85250d6 + 0 + 0) // checksum
};

void kernel_main() {
    clear_screen();

    kprint("Welcome to IronicOS!\n");
    kprint("Initializing system...\n");

    isr_install();         // Setup ISRs (exceptions 0-31)
    irq_install();         // Setup IRQs (32-47)
    init_keyboard();       // Enable keyboard handler (IRQ1)

    kprint("System initialized successfully.\n");
    kprint("Type something on the keyboard...\n");

    // Infinite loop to keep kernel running
    while (1) {
        __asm__ __volatile__("hlt");
    }
}