#include "../lib/screen.h"  // For kprint()
#include "../lib/string.h"
#include "isr.h"
#include "idt.h"
#include "ports.h"

isr_handler_t interrupt_handlers[256];  // Can be used for ISRs and IRQs

const char *exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
    "Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved"
};

void isr_install() {
    idt_init();  // Initialize and load the IDT
}

void isr_handler(registers_t *regs) {
    if (interrupt_handlers[regs->int_no] != 0) {
        isr_handler_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    } else {
        kprint("Unhandled Exception: ");
        kprint(exception_messages[regs->int_no]);
        kprint("\nSystem Halted!\n");
        while (1) __asm__ __volatile__("hlt");
    }
}

void isr_register_handler(uint8_t n, isr_handler_t handler) {
    if (n < 256) {
        interrupt_handlers[n] = handler;
    }
}

void isr_unregister_handler(uint8_t n) {
    if (n < 256) {
        interrupt_handlers[n] = 0;
    }
}

void isr_set_handler(int n, isr_handler_t handler) {
    if (n < 256) {
        interrupt_handlers[n] = handler;
    }
}