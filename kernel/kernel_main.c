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
    // Test harness expects this exact line
    print("Booting IronicOS\n");

    // Your regular debug prints
    print("[IronicOS] Setting up IDT...\n");
    idt_init();
    print("[IronicOS] Installing ISRs...\n");
    isr_install();
    print("[IronicOS] Installing IRQs...\n");
    irq_install();
    print("[IronicOS] Initializing keyboard...\n");
    keyboard_init();
    print("[IronicOS] Initializing timer at 100Hz...cv\n");
    timer_init(100);

    // Your kernel main loop or shell...
}
