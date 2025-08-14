// lib/serial.c
#include "serial.h"
#include <stddef.h>

#define COM1_PORT 0x3F8   // Standard COM1

static int serial_initialized = 0;

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(COM1_PORT + 1, 0x00); // Disable interrupts
    outb(COM1_PORT + 3, 0x80); // Enable DLAB
    outb(COM1_PORT + 0, 0x03); // Baud rate divisor low byte (38400 baud)
    outb(COM1_PORT + 1, 0x00); // Baud rate divisor high byte
    outb(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, 14-byte threshold
    outb(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set

    serial_initialized = 1;
}

static int serial_is_transmit_empty(void) {
    return inb(COM1_PORT + 5) & 0x20;
}

void serial_write_char(char a) {
    if (!serial_initialized) serial_init();
    while (!serial_is_transmit_empty());
    outb(COM1_PORT, a);
}

void serial_puts(const char *str) {
    if (!serial_initialized) serial_init();
    if (!str) return;
    while (*str) {
        if (*str == '\n')
            serial_write_char('\r');
        serial_write_char(*str++);
    }
}
