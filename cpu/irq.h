#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"    /* reuses registers_t */

typedef void (*irq_handler_t)(registers_t* r);

/* Assembly IRQ stubs (for IDT 32..47 after PIC remap) */
extern void irq0();  extern void irq1();  extern void irq2();  extern void irq3();
extern void irq4();  extern void irq5();  extern void irq6();  extern void irq7();
extern void irq8();  extern void irq9();  extern void irq10(); extern void irq11();
extern void irq12(); extern void irq13(); extern void irq14(); extern void irq15();

/* Called by asm common stub */
void irq_dispatch(registers_t* r);

/* Your irq.c API */
void register_irq_handler(uint8_t irq, irq_handler_t handler);
void irq_install_handler(uint8_t irq, irq_handler_t handler);
void irq_uninstall_handler(uint8_t irq);
void irq_remap(void);
void init_irq(void);

#endif