#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"  // For `registers_t`

#define IRQ_BASE 32

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

// IRQ handler function pointer type
typedef void (*irq_handler_t)(registers_t *);

// Install the IRQs
void irq_install();

// Register/unregister a custom handler
void irq_register_handler(uint8_t irq, irq_handler_t handler);
void irq_unregister_handler(uint8_t irq);

#endif