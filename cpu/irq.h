#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"

void irq_remap();
void irq_install();
void irq_handler(registers_t regs);

#endif