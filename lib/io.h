#ifndef IRT_IO_H
#define IRT_IO_H

#include <stdarg.h>

/* Minimal printf-like API used around the kernel.
   Implemented in screen.c; other modules call printf(). */
void printf(const char *fmt, ...);
void putchar(char c);
void clear_screen(void);

#endif /* IRT_IO_H */
