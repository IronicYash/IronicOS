#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

// Function declarations for port I/O operations
void port_byte_out(uint16_t port, uint8_t data);
uint8_t port_byte_in(uint16_t port);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

#endif