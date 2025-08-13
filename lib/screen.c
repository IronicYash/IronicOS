#include "screen.h"
#include "ports.h"
#include <stdarg.h>
#include <stdint.h>

/* VGA text-mode framebuffer */
static volatile uint16_t *vga = (volatile uint16_t*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;
static uint8_t attribute = 0x0F; /* white on black */

static void update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void clear_screen(void) {
    for (int y = 0; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            vga[y * VGA_WIDTH + x] = (attribute << 8) | ' ';
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        vga[cursor_y * VGA_WIDTH + cursor_x] = (attribute << 8) | (uint8_t)c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    if (cursor_y >= VGA_HEIGHT) {
        /* scroll up by one line */
        for (int y = 1; y < VGA_HEIGHT; ++y) {
            for (int x = 0; x < VGA_WIDTH; ++x) {
                vga[(y - 1) * VGA_WIDTH + x] = vga[y * VGA_WIDTH + x];
            }
        }
        /* clear last line */
        for (int x = 0; x < VGA_WIDTH; ++x)
            vga[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (attribute << 8) | ' ';
        cursor_y = VGA_HEIGHT - 1;
    }
    update_cursor();
}

void puts(const char *s) {
    while (*s) putchar(*s++);
}

/* Minimal integer --> string helpers */
static void itoa_dec(int value, char *buf) {
    char tmp[16];
    int i = 0;
    if (value == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    int neg = 0;
    if (value < 0) { neg = 1; value = -value; }
    while (value) {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    }
    if (neg) tmp[i++] = '-';
    int j = 0;
    while (i--) buf[j++] = tmp[i];
    buf[j] = '\0';
}

static void itoa_hex(unsigned int value, char *buf) {
    const char *hex = "0123456789ABCDEF";
    char tmp[16];
    int i = 0;
    if (value == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (value) {
        tmp[i++] = hex[value & 0xF];
        value >>= 4;
    }
    int j = 0;
    while (i--) buf[j++] = tmp[i];
    buf[j] = '\0';
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char ch;
    while ((ch = *fmt++)) {
        if (ch != '%') {
            putchar(ch);
            continue;
        }
        char spec = *fmt++;
        if (!spec) break;
        switch (spec) {
            case 'c': {
                char c = (char)va_arg(args, int);
                putchar(c);
                break;
            }
            case 's': {
                char *s = va_arg(args, char*);
                if (!s) s = "(null)";
                puts(s);
                break;
            }
            case 'd':
            case 'i': {
                int v = va_arg(args, int);
                char buf[16];
                itoa_dec(v, buf);
                puts(buf);
                break;
            }
            case 'u': {
                unsigned int v = va_arg(args, unsigned int);
                char buf[16];
                itoa_dec((int)v, buf);
                puts(buf);
                break;
            }
            case 'x':
            case 'X': {
                unsigned int v = va_arg(args, unsigned int);
                char buf[16];
                itoa_hex(v, buf);
                puts(buf);
                break;
            }
            case '%': putchar('%'); break;
            default:
                putchar('%');
                putchar(spec);
        }
    }
    va_end(args);
}