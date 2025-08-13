#ifndef IRT_SCREEN_H
#define IRT_SCREEN_H

#include <stdint.h>

void clear_screen(void);
void putchar(char c);
void puts(const char *s);
void printf(const char *fmt, ...);

#endif /* IRT_SCREEN_H */