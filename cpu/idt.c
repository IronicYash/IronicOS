#include "idt.h"
#include <stdint.h>

extern void idt_load(void*);  // implemented in idt_load.asm

// External ISR and IRQ stub tables
extern void* isr_stub_table;
extern void* irq_stub_table;

struct idt_entry idt[256];
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_ptr idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

void idt_init() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

    // Initialize all IDT entries to 0
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Set up ISR handlers (exceptions 0-31)
    void (**isr_stubs)(void) = (void (**)(void))&isr_stub_table;
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)isr_stubs[i], 0x08, 0x8E);
    }

    // Set up IRQ handlers (interrupts 32-47)
    void (**irq_stubs)(void) = (void (**)(void))&irq_stub_table;
    for (int i = 0; i < 16; i++) {
        idt_set_gate(i + 32, (uint32_t)irq_stubs[i], 0x08, 0x8E);
    }

    idt_load(&idtp); // Pass pointer directly to asm
}