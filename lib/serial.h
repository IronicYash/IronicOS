// lib/serial.h
#ifndef SERIAL_H
#define SERIAL_H

void serial_init(void);
void serial_write_char(char a);
void serial_puts(const char *str);

#endif
