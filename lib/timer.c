#include "timer.h"
#include "../cpu/irq.h"
#include "../cpu/isr.h"
#include "ports.h"
#include "screen.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182

static uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    // Optional: Uncomment to print tick
    // kprint("Tick: ");
    // kprint_dec(tick);
    // kprint("\n");
}

void init_timer(uint32_t frequency) {
    register_interrupt_handler(32, timer_callback);

    uint32_t divisor = PIT_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36); // Binary mode, square wave, channel 0
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));         // Low byte
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));  // High byte
}

uint32_t timer_get_ticks() {
    return tick;
}

void timer_sleep(uint32_t ticks_to_wait) {
    uint32_t start = tick;
    while ((tick - start) < ticks_to_wait) {
        __asm__ volatile ("hlt");  // Sleep CPU until next interrupt
    }
}