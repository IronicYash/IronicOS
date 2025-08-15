// Simple, robust serial driver for COM1 (0x3F8)
// Production-style: minimal, inline asm, idempotent init.

#include "serial.h"

#define COM1_PORT 0x3F8

static int serial_initialized = 0;

static inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void serial_init(void) {
    if (serial_initialized) return;

    outb(COM1_PORT + 1, 0x00);    // Disable interrupts
    outb(COM1_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1_PORT + 0, 0x03);    // Divisor low byte (38400 baud)
    outb(COM1_PORT + 1, 0x00);    // Divisor high byte
    outb(COM1_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1_PORT + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(COM1_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

    serial_initialized = 1;
}

static int serial_is_transmit_empty(void) {
    return inb(COM1_PORT + 5) & 0x20;
}

void serial_write_char(char c) {
    if (!serial_initialized) serial_init();
    while (!serial_is_transmit_empty());
    outb(COM1_PORT, (unsigned char)c);
}

void serial_puts(const char *str) {
    if (!str) return;
    if (!serial_initialized) serial_init();
    while (*str) {
        char c = *str++;
        if (c == '\n') serial_write_char('\r');
        serial_write_char(c);
    }
}

void serial_puts_n(const char *str, size_t n) {
    if (!str) return;
    if (!serial_initialized) serial_init();
    for (size_t i = 0; i < n; ++i) {
        char c = str[i];
        if (c == '\n') serial_write_char('\r');
        serial_write_char(c);
    }
}
