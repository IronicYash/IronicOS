#include "../lib/screen.h"
#include "../lib/keyboard.h"
#include "../lib/ports.h"
#include "../lib/timer.h"
#include "../lib/shell.h"
#include "../lib/serial.h"
#include "../lib/string.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/irq.h"
#include <stdint.h>


void kernel_main() {
    // Test harness expects this exact line
    serial_puts("Booting IronicOS\n");

    // Your debug prints
    serial_puts("[IronicOS] Setting up IDT...\n");
    idt_init();

    serial_puts("[IronicOS] Installing ISRs...\n");
    init_isr();

    serial_puts("[IronicOS] Installing IRQs...\n");
    init_irq();

    serial_puts("[IronicOS] Initializing keyboard...\n");
    init_keyboard();

    serial_puts("[IronicOS] Initializing timer at 100Hz...\n");
    init_timer(100);

    // Example: infinite loop or shell
    serial_puts("[IronicOS] Kernel initialized. Entering main loop...\n");
    while (1) {
        // Your shell or main kernel loop goes here
		shell_init();
		shell_run();
        __asm__ volatile ("hlt");
    }
}
