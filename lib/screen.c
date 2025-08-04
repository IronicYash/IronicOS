#include "screen.h"
#include "io.h"          // For outb()
#include <stdint.h>

#define VIDEO_MEMORY ((uint16_t*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// ✅ Global static variables for cursor position
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

// ✅ Update hardware cursor
static void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// ✅ Print a single character at the current cursor location
void print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (uint16_t)c | (0x07 << 8);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        clear_screen(); // For now, just clear when bottom is reached
    }

    update_cursor();
}

// ✅ Print string using print_char
void print(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

// ✅ Clear screen
void clear_screen() {
    uint16_t blank = ' ' | (0x07 << 8);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VIDEO_MEMORY[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}