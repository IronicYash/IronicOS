#include "irq.h"
#include "../lib/serial.h"
#include <stddef.h>
#include <stdint.h>

/* from cpu/isr_irq_stubs.asm */
extern void* irq_stub_table[16];

/* I/O port helpers (local, no global header dependency) */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" :: "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* 8259 PIC ports */
#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI   0x20

/* IRQ callbacks for 0..15 */
static irq_callback_t g_irq_handlers[16];

/* Basic PIC remap to vectors 0x20..0x2F */
static void pic_remap(void) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    /* starts the initialization sequence (in cascade mode) */
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    /* ICW2: Master offset 0x20, Slave offset 0x28 */
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    /* ICW3: tell Master that there is a slave at IRQ2 (0000 0100), and slave its cascade identity (2) */
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    /* ICW4: 8086/88 (MCS-80/85) mode */
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    /* restore saved masks */
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

/* Optionally unmask a specific IRQ line */
static void irq_clear_mask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    uint8_t value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void init_irq(void) {
    for (int i = 0; i < 16; ++i) g_irq_handlers[i] = NULL;

    /* Remap PIC to 0x20..0x2F */
    pic_remap();

    /* Example: unmask PIT timer (IRQ0) and keyboard (IRQ1) if you need them early */
    irq_clear_mask(0); /* timer */
    irq_clear_mask(1); /* keyboard */
}

void register_irq_handler(uint8_t irq, irq_callback_t cb) {
    if (irq < 16) g_irq_handlers[irq] = cb;
}

/* Called from irq_common_stub with vector number (32..47) */
void irq_handler_c(uint32_t vec) {
    if (vec < 32 || vec > 47) {
        serial_puts("[IRQ] Spurious or invalid vector: ");
        serial_puthex(vec);
        serial_puts("\n");
        return;
    }
    uint8_t irq = (uint8_t)(vec - 32);

    /* Dispatch to registered handler if present */
    if (g_irq_handlers[irq]) {
        g_irq_handlers[irq](irq);
    } else {
        serial_puts("[IRQ] Unhandled IRQ ");
        serial_puthex(irq);
        serial_puts("\n");
    }

    /* Send End Of Interrupt (EOI) to PIC(s) */
    if (irq >= 8) outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}
