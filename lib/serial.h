#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

void serial_init(void);
void serial_write_char(char a);
void serial_puts(const char *str);
void serial_puts_n(const char *str, size_t n);

#endif /* SERIAL_H */
