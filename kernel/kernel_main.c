#include "../lib/screen.h"
#include "../lib/keyboard.h"
#include "../lib/timer.h"
#include "../lib/io.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/irq.h"


void kernel_main() {
    clear_screen();
    print("Welcome to IronicOS!\n");

    idt_init();         // Setup IDT
    isr_install();      // Setup ISRs
    irq_install();      // Setup IRQs
    keyboard_init();    // Enable keyboard interrupt
    
    init_timer(100);    // Initialize timer at 100 Hz

}