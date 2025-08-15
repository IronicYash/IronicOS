// lib/serial.c
#include "serial.h"
#include "string.h" // for strlen etc
#include "ports.h"

#define SERIAL_PORT 0x3F8 // COM1

void serial_init() {
    outb(SERIAL_PORT + 1, 0x00); // disable interrupts
    outb(SERIAL_PORT + 3, 0x80); // enable DLAB
    outb(SERIAL_PORT + 0, 0x03); // baud rate divisor (low)
    outb(SERIAL_PORT + 1, 0x00); // baud rate divisor (high)
    outb(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7); // FIFO
    outb(SERIAL_PORT + 4, 0x0B); // modem control
}

int serial_received() {
    return inb(SERIAL_PORT + 5) & 1;
}

char serial_getc() {
    while (!serial_received());
    return inb(SERIAL_PORT);
}

int is_transmit_empty() {
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_putc(char c) {
    while (!is_transmit_empty());
    outb(SERIAL_PORT, c);
}

void serial_puts(const char* str) {
    for (size_t i = 0; i < strlen(str); i++)
        serial_putc(str[i]);
}

void serial_put_hex(uint32_t n) {
    char hex[9];
    for (int i = 0; i < 8; i++) {
        int val = (n >> ((7 - i) * 4)) & 0xF;
        if (val < 10) hex[i] = '0' + val;
        else hex[i] = 'A' + (val - 10);
    }
    hex[8] = 0;
    serial_puts(hex);
}

void serial_put_dec(uint32_t n) {
    char buf[12];
    int i = 10;
    buf[11] = 0;
    if (n == 0) { serial_putc('0'); return; }
    while (n > 0 && i >= 0) {
        buf[i--] = '0' + (n % 10);
        n /= 10;
    }
    serial_puts(buf + i + 1);
}
