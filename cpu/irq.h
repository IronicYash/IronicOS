#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*irq_callback_t)(uint32_t irq);

/* Top-level handler called from ASM for PIC IRQs (32..47) */
void irq_handler_c(uint32_t vec);

/* Init and registration for IRQ handlers */
void init_irq(void);
void register_irq_handler(uint8_t irq, irq_callback_t cb);

/* ASM-provided table of IRQ stubs (0..15), mapped to IDT vectors 32..47 */
extern void* irq_stub_table[16];

#ifdef __cplusplus
}
#endif
