#include "irq.h"
#include "../lib/ports.h"
#include "../lib/screen.h"
#include "../lib/string.h"

/* PIC ports */
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

/* Table of IRQ stub addresses for IDT setup */
void* irq_stub_table[16] = {
    irq0,  irq1,  irq2,  irq3,
    irq4,  irq5,  irq6,  irq7,
    irq8,  irq9,  irq10, irq11,
    irq12, irq13, irq14, irq15
};


static irq_handler_t irq_routines[16];

/* Default handler if none registered */
static void irq_default(struct registers *r)
{
    /* simple ack and return */
    (void)r;
}

/* Register and uninstall helpers */
void register_irq_handler(uint8_t irq, irq_handler_t handler)
{
    irq_routines[irq] = handler;
}

void irq_install_handler(uint8_t irq, irq_handler_t handler)
{
    register_irq_handler(irq, handler);
}

void irq_uninstall_handler(uint8_t irq)
{
    irq_routines[irq] = 0;
}

/* Common IRQ dispatch called from assembly */
void irq_dispatch(struct registers *r)
{
    uint8_t irq_no = r->int_no - 32; /* IRQs remapped to 32..47 */
    if (irq_no < 16 && irq_routines[irq_no]) {
        irq_routines[irq_no](r);
    } else {
        /* No handler; optional debug */
        // printf("IRQ %d fired\n", irq_no);
    }

    /* Send End of Interrupt (EOI) to PICs */
    if (irq_no >= 8) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}

/* Remap PIC 0..15 to 32..47 */
void irq_remap(void)
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA); // save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11); // start init
    outb(PIC2_COMMAND, 0x11);

    outb(PIC1_DATA, 0x20); // offset PIC1 to 0x20 (32)
    outb(PIC2_DATA, 0x28); // offset PIC2 to 0x28 (40)

    outb(PIC1_DATA, 0x04); // tell PIC1 about PIC2 at IRQ2
    outb(PIC2_DATA, 0x02); // tell PIC2 its cascade identity

    outb(PIC1_DATA, 0x01); // 8086/88 (MCS-80/85) mode
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, a1); // restore saved masks
    outb(PIC2_DATA, a2);
}

void init_irq(void)
{
    /* Zero handlers */
    for (int i = 0; i < 16; ++i) irq_routines[i] = 0;

    irq_remap();

    /* Fill IDT entries 32..47 with IRQ stubs (set_idt_gate should be called elsewhere) */
}