#ifndef IRT_TIMER_H
#define IRT_TIMER_H

#include <stdint.h>

/* init_timer - set PIT frequency in Hz */
void init_timer(uint32_t frequency);

/* simple sleep (busy/hlt loop) in milliseconds */
void mdelay(uint32_t ms);

#endif /* IRT_TIMER_H */