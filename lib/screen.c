#include "screen.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}


static uint16_t* const video_memory = (uint16_t*) 0xB8000;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

static void update_cursor() {
    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void print_char(char c) {
    if (c == '\n') {
        new_line();
    } else {
        const uint16_t index = cursor_row * VGA_WIDTH + cursor_col;
        video_memory[index] = ((uint8_t)WHITE_ON_BLACK << 8) | c;
        cursor_col++;

        if (cursor_col >= VGA_WIDTH) {
            new_line();
        }
    }

    update_cursor();
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

void new_line() {
    cursor_col = 0;
    cursor_row++;

    if (cursor_row >= VGA_HEIGHT) {
        // Scroll screen up
        for (int row = 1; row < VGA_HEIGHT; row++) {
            for (int col = 0; col < VGA_WIDTH; col++) {
                video_memory[(row - 1) * VGA_WIDTH + col] = video_memory[row * VGA_WIDTH + col];
            }
        }

        // Clear last line
        for (int col = 0; col < VGA_WIDTH; col++) {
            video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = ((uint8_t)WHITE_ON_BLACK << 8) | ' ';
        }

        cursor_row = VGA_HEIGHT - 1;
    }
}

void clear_screen() {
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            video_memory[row * VGA_WIDTH + col] = ((uint8_t)WHITE_ON_BLACK << 8) | ' ';
        }
    }

    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}