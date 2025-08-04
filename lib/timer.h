#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Initialize the PIT to a specified frequency (e.g. 100 Hz)
void init_timer(uint32_t frequency);

// Get number of ticks since boot
uint32_t timer_get_ticks();

// Sleep for a specific number of ticks
void timer_sleep(uint32_t ticks);

#endif