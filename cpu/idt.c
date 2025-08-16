// cpu/idt.c
// Ready-to-drop IDT implementation for 32-bit protected mode
// Adjust include paths to match your project layout.

#include <stdint.h>
#include "../lib/serial.h"   // adjust path if needed

// IDT entry - must be packed and exactly 8 bytes
typedef struct __attribute__((packed)) {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} idt_entry_t;

// IDT pointer (for lidt)
typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} idt_ptr_t;

// 256 entries (full IDT)
idt_entry_t idt[256];
idt_ptr_t idtp;

// External assembly function that executes `lidt [idtp]`
extern void idt_load(void);

// If you have arrays of ISR/IRQ stubs created in assembly, declare them here.
// Example: extern void isr_stub_0(); extern void irq_stub_0();
// If you provide tables, declare them like:
extern void (*isr_stub_table[])(void);
extern void (*irq_stub_table[])(void);

// Helper to set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (uint16_t)(base & 0xFFFF);
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (uint16_t)((base >> 16) & 0xFFFF);
}

// Simple memzero for the IDT area (avoid depending on libc)
static void idt_memzero(void *dst, uint32_t len) {
    uint8_t *p = (uint8_t *)dst;
    for (uint32_t i = 0; i < len; ++i) p[i] = 0;
}

// Debug helper: print a 32-bit hex value (assumes serial_puthex exists)
// If you don't have serial_puthex, you can implement one in lib/serial.

// Public init function
void idt_init(uint16_t kernel_cs_selector) {
    // Clear IDT
    idt_memzero(idt, sizeof(idt));

    // Setup IDT pointer
    idtp.limit = (uint16_t)(sizeof(idt_entry_t) * 256 - 1);
    idtp.base = (uint32_t)&idt;

    serial_puts("[idt] init: base=");
    serial_puthex(idtp.base);
    serial_puts(" limit=");
    serial_puthex(idtp.limit);
    serial_puts(" cs=");
    serial_puthex(kernel_cs_selector);
    serial_puts("\n");

    // If you have ISR stubs (0-31) and IRQ stubs (32-47), populate them here.
    // This code assumes you provide arrays named isr_stub_table and irq_stub_table
    // with the correct number of function pointers. If you don't, comment this block
    // and manually set gates to your handler symbols.

    // Install CPU exception handlers (vectors 0..31)
    for (uint8_t i = 0; i < 32; ++i) {
        uint32_t handler = (uint32_t)isr_stub_table[i];
        idt_set_gate(i, handler, kernel_cs_selector, 0x8E); // present, ring0, 32-bit interrupt gate
    }

    // Install IRQ handlers (vectors 32..47)
    for (uint8_t i = 0; i < 16; ++i) {
        uint32_t handler = (uint32_t)irq_stub_table[i];
        idt_set_gate(32 + i, handler, kernel_cs_selector, 0x8E);
    }

    // Optionally: other software interrupts, syscall gate (ring3) etc.

    // Load the new IDT
    idt_load();

    serial_puts("[idt] loaded\n");
}

// Small function to inspect one IDT entry (for debugging)
uint32_t idt_get_handler_addr(uint8_t num) {
    uint32_t low = idt[num].base_low;
    uint32_t high = idt[num].base_high;
    return (high << 16) | low;
}
