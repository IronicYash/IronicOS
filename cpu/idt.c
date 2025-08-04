#include "idt.h"
#include "isr.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern void idt_load(); // Defined in entry.asm

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0); // Clear all entries
    }

    isr_install();
    irq_install();

    idt_load(); // Load the new IDT
}