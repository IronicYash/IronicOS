#include "isr.h"
#include "../lib/serial.h"
#include <stddef.h>

extern void* isr_stub_table[32]; /* from cpu/isr_irq_stubs.asm */

static isr_callback_t g_isr_handlers[32];

static void default_exception_handler(uint32_t vec) {
    serial_puts("[EXC] Unhandled exception vec=");
    serial_puthex(vec);
    serial_puts("\n");
    /* Hang with interrupts disabled to avoid storm */
    asm volatile ("cli; hlt");
    for (;;) { asm volatile ("hlt"); }
}

void init_isr(void) {
    for (int i = 0; i < 32; ++i) g_isr_handlers[i] = NULL;

    /* You can pre-install some handlers here if you want special cases,
       e.g., divide-by-zero (0), page fault (14), etc. */
}

void register_exception_handler(uint8_t vec, isr_callback_t cb) {
    if (vec < 32) g_isr_handlers[vec] = cb;
}

/* Called from isr_common_stub with vector number (0..31) */
void isr_handler_c(uint32_t vec) {
    if (vec < 32 && g_isr_handlers[vec]) {
        g_isr_handlers[vec](vec);
    } else {
        default_exception_handler(vec);
    }
}
