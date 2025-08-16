#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*isr_callback_t)(uint32_t vec);

/* Top-level handler called from ASM for CPU exceptions (0..31) */
void isr_handler_c(uint32_t vec);

/* Init and registration for exception handlers */
void init_isr(void);
void register_exception_handler(uint8_t vec, isr_callback_t cb);

/* ASM-provided table of exception stubs (0..31) */
extern void* isr_stub_table[32];

#ifdef __cplusplus
}
#endif
