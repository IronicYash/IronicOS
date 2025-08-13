#ifndef IRT_KEYBOARD_H
#define IRT_KEYBOARD_H

#include <stdint.h>

/* Initialize PS/2 keyboard (enable IRQ) */
void init_keyboard(void);

/* Non-blocking read of latest character (0 if none) */
int keyboard_getchar(void);

/* Blocking read (simple polling) */
int keyboard_getchar_blocking(void);

#endif /* IRT_KEYBOARD_H */