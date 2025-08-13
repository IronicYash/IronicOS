/* lib/serial.c -- very small polled serial for early debug */

static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    const unsigned short COM1 = 0x3F8;
    outb(COM1+1, 0x00);    /* Disable all interrupts */
    outb(COM1+3, 0x80);    /* Enable DLAB */
    outb(COM1+0, 0x03);    /* Divisor low 3 => 38400 (works) */
    outb(COM1+1, 0x00);    /* Divisor high */
    outb(COM1+3, 0x03);    /* 8 bits, no parity, one stop bit */
    outb(COM1+2, 0xC7);    /* Enable FIFO, clear them */
    outb(COM1+4, 0x0B);    /* IRQs enabled, RTS/DSR set */
}

void serial_putc(char c) {
    const unsigned short COM1 = 0x3F8;
    while ((inb(COM1+5) & 0x20) == 0) { } /* wait for THR empty */
    outb(COM1, c);
}

void serial_puts(const char *s) {
    while (*s) serial_putc(*s++);
}