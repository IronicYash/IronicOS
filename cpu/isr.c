#include "isr.h"
#include "idt.h"
#include "../lib/screen.h"

extern void isr_stub_table(); // All 32 ISR stubs defined in ASM
extern void* isr_stub_table[32];

isr_t interrupt_handlers[256];

void isr_install() {
    extern void* isr_stub_table[];

    for (int i = 0; i < ISR_COUNT; i++) {
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no]) {
        interrupt_handlers[regs.int_no](regs);
    } else {
        kprint("Unhandled CPU Exception: ");
        kprint_dec(regs.int_no);
        kprint("\n");
    }
}