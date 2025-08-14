#include "../lib/screen.h"
#include "../lib/keyboard.h"
#include "../lib/ports.h"
#include "../lib/timer.h"
#include "../lib/shell.h"
#include "../lib/io.h"
#include "../lib/serial.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/irq.h"
#include <stdint.h>

uint8_t example_byte;
uint32_t count;

extern void serial_init(void);
extern void serial_puts(const char *);

void kernel_main() {
    serial_puts("Booting IronicOS via QEMU Serial...\n");

    clear_screen();
    printf("Welcome to IronicOS!\n");

    idt_init();         // Setup IDT
    init_irq();      // Setup ISRs
    init_isr();      // Setup IRQs
    init_keyboard();    // Enable keyboard interrupt

    init_timer(100);    // Initialize timer at 100 Hz

    shell_loop();       // Initialize the shell
}
