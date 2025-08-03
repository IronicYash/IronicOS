#include "irq.h"
#include "idt.h"
#include "ports.h"
#include "../lib/screen.h"

// Array to store custom IRQ handlers
irq_handler_t irq_handlers[16] = { 0 };

void irq_remap() {
    // Send ICW1
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // Send ICW2 - Master PIC offset
    outb(0x21, 0x20);  // Remap IRQs 0-7 to IDT entries 32-39
    outb(0xA1, 0x28);  // Remap IRQs 8-15 to IDT entries 40-47

    // Send ICW3
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Send ICW4
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Unmask all interrupts
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

// Install IRQs into the IDT
void irq_install() {
    irq_remap();

    extern void irq0();
    extern void irq1();
    extern void irq2();
    extern void irq3();
    extern void irq4();
    extern void irq5();
    extern void irq6();
    extern void irq7();
    extern void irq8();
    extern void irq9();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();

    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);
}

// C entry point for IRQs
void irq_handler(registers_t *r) {
    // Call custom handler if registered
    uint8_t irq = r->int_no - IRQ_BASE;
    if (irq < 16 && irq_handlers[irq]) {
        irq_handlers[irq](r);
    }

    // Send End of Interrupt (EOI) to the PICs
    if (r->int_no >= 40) {
        outb(0xA0, 0x20);  // Slave PIC
    }
    outb(0x20, 0x20);      // Master PIC
}

// Register a handler for a specific IRQ
void irq_register_handler(uint8_t irq, irq_handler_t handler) {
    if (irq < 16) {
        irq_handlers[irq] = handler;
    }
}

// Unregister handler
void irq_unregister_handler(uint8_t irq) {
    if (irq < 16) {
        irq_handlers[irq] = 0;
    }
}