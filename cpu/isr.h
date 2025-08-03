#ifndef ISR_H
#define ISR_H

#include <stdint.h>

#define ISR_COUNT 32

// Registers pushed by the CPU during ISR
typedef struct {
    uint32_t ds;                                     // Data segment
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                       // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed automatically by the processor
} registers_t;

typedef void (*isr_handler_t)(registers_t *);

void isr_install();
void isr_register_handler(uint8_t n, isr_handler_t handler);
void isr_unregister_handler(uint8_t n);
void isr_set_handler(int n, isr_handler_t handler);

#endif
