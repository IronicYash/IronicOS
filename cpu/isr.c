#include "isr.h"
#include "../lib/screen.h"  /* for debug output, printf */
#include "../lib/string.h"

/* Table of ISR stub addresses for IDT setup */
void* isr_stub_table[32] = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,
    isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

static isr_t isr_handlers[32];

void register_isr_handler(uint8_t n, isr_t handler)
{
    if (n < 32) isr_handlers[n] = handler;
}

void isr_default_handler(struct registers *r)
{
    /* Basic fault reporting */
    clear_screen();
    printf("Unhandled exception: %d\n", r->int_no);
    printf("EIP: 0x%08X  CS: 0x%08X  EFLAGS: 0x%08X\n", r->eip, r->cs, r->eflags);

    /* Halt */
    for (;;)
        asm volatile ("hlt");
}

/* This function is called from assembly stubs */
void isr_dispatch(struct registers *r)
{
    if (r->int_no < 32 && isr_handlers[r->int_no]) {
        isr_handlers[r->int_no](r);
    } else {
        isr_default_handler(r);
    }
}

void init_isr(void)
{
    /* Register default handlers (optionally override later) */
    for (int i = 0; i < 32; ++i)
        isr_handlers[i] = 0;

    /* The assembly will point IDT entries 0..31 to stubs in isr.asm.
       Ensure your idt setup calls set_idt_gate for these with proper flags. */
}