#include "timer.h"
#include "ports.h"
#include "screen.h"
#include <stdint.h>

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_FREQ 1193182u

static volatile uint32_t ticks = 0;

void timer_handler(void *r) {
    (void)r;
    ticks++;
}

/* We'll rely on the irq system to call this handler at IRQ0 */
void init_timer(uint32_t frequency) {
    uint32_t divisor = PIT_FREQ / frequency;
    outb(PIT_COMMAND, 0x36); /* Channel 0, low/high, mode 3 */
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
    /* Users should register the C-level handler via irq_install_handler(0, timer_handler); */
}

void mdelay(uint32_t ms) {
    uint32_t target = ticks + (ms / (1000 / 100)); /* rough: assuming 100Hz if set */
    while (ticks < target) asm volatile("hlt");
}