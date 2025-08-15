#include "../lib/screen.h"
#include "../lib/keyboard.h"
#include "../lib/ports.h"
#include "../lib/timer.h"
#include "../lib/shell.h"
#include "../lib/serial.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/irq.h"
#include <stdint.h>

void kernel_main() {
    // ===== Stage 1: Early serial init =====
    serial_init();
    serial_puts("[IronicOS] Booting via QEMU serial...\n");

    // ===== Stage 2: VGA clear and welcome =====
    clear_screen();
    printf("[IronicOS] VGA output online.\n");

    // ===== Stage 3: IDT and ISRs =====
    printf("[IronicOS] Setting up IDT...\n");
    serial_puts("[IronicOS] Setting up IDT...\n");
    idt_init();

    printf("[IronicOS] Installing ISRs...\n");
    serial_puts("[IronicOS] Installing ISRs...\n");
    init_isr();

    printf("[IronicOS] Installing IRQs...\n");
    serial_puts("[IronicOS] Installing IRQs...\n");
    init_irq();

    // ===== Stage 4: Device init =====
    printf("[IronicOS] Initializing keyboard...\n");
    serial_puts("[IronicOS] Initializing keyboard...\n");
    init_keyboard();

    printf("[IronicOS] Initializing timer at 100Hz...\n");
    serial_puts("[IronicOS] Initializing timer at 100Hz...\n");
    init_timer(100);

    // ===== Stage 5: Shell =====
    printf("[IronicOS] Entering shell...\n");
    serial_puts("[IronicOS] Entering shell...\n");
    shell_loop();

    // If shell exits
    printf("[IronicOS] Shell exited. Halting.\n");
    serial_puts("[IronicOS] Shell exited. Halting.\n");
    for(;;) { __asm__ volatile("hlt"); }
}
