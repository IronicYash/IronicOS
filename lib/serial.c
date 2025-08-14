#include "serial.h"
#include "ports.h"

#define COM1 0x3F8

void serial_init(void) {
    outb(COM1 + 1, 0x00);    // Disable interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB
    outb(COM1 + 0, 0x03);    // Baud rate divisor low byte (38400 baud)
    outb(COM1 + 1, 0x00);    // Baud rate divisor high byte
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static int serial_is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

void serial_write_char(char a) {
    while (!serial_is_transmit_empty());
    outb(COM1, a);
}

void serial_write_string(const char* str) {
    while (*str) {
        serial_write_char(*str++);
    }
}
