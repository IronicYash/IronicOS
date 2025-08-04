#include "irq.h"
#include "idt.h"
#include "../lib/ports.h"

#define IRQ_BASE 32
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)
#define PIC_EOI 0x20

void irq_remap() {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, IRQ_BASE);      // Remap PIC1 to 32–39
    outb(PIC2_DATA, IRQ_BASE + 8);  // Remap PIC2 to 40–47
    outb(PIC1_DATA, 4);             // Tell PIC1 there is a PIC2 at IRQ2
    outb(PIC2_DATA, 2);             // Tell PIC2 its cascade identity
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
}

void irq_install() {
    irq_remap();
    extern void* irq_stub_table[];

    for (int i = 0; i < 16; i++) {
        idt_set_gate(IRQ_BASE + i, (uint32_t)irq_stub_table[i], 0x08, 0x8E);
    }
}

void irq_handler(registers_t regs) {
    if (regs.int_no >= IRQ_BASE && interrupt_handlers[regs.int_no]) {
        interrupt_handlers[regs.int_no](regs);
    }

    // Send EOI
    if (regs.int_no >= IRQ_BASE + 8)
        outb(PIC2_COMMAND, PIC_EOI);

    outb(PIC1_COMMAND, PIC_EOI);
}
