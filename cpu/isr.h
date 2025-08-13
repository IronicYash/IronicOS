#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef struct registers {
    uint32_t ds;                  /* saved by asm stub */

    /* General-purpose registers (order matches our asm: we use pushad, but we don't rely on exact order in C code) */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    /* From asm stubs (uniform for all ISRs): */
    uint32_t int_no, err_code;

    /* Pushed by CPU automatically: */
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/* Type used by your isr.c */
typedef void (*isr_t)(registers_t* r);

/* Assembly stubs (used when setting IDT gates) */
extern void isr0();  extern void isr1();  extern void isr2();  extern void isr3();
extern void isr4();  extern void isr5();  extern void isr6();  extern void isr7();
extern void isr8();  extern void isr9();  extern void isr10(); extern void isr11();
extern void isr12(); extern void isr13(); extern void isr14(); extern void isr15();
extern void isr16(); extern void isr17(); extern void isr18(); extern void isr19();
extern void isr20(); extern void isr21(); extern void isr22(); extern void isr23();
extern void isr24(); extern void isr25(); extern void isr26(); extern void isr27();
extern void isr28(); extern void isr29(); extern void isr30(); extern void isr31();

/* Called by asm common stub */
void isr_dispatch(registers_t* r);

/* Your isr.c API */
void register_isr_handler(uint8_t n, isr_t handler);
void isr_default_handler(struct registers *r);
void init_isr(void);

#endif