#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void serial_init();
void serial_putc(char c);
void serial_puts(const char* str);
void serial_put_hex(uint32_t n);
void serial_put_dec(uint32_t n);
char serial_getc();

#endif
