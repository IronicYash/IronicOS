#include "screen.h"
#include "../cpu/ports.h"

static uint16_t* const video_memory = (uint16_t*) 0xB8000;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

#include "../cpu/ports.h"

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

int get_cursor_offset() {
    outb(0x3D4, 14); // Tell VGA control register: we're accessing the high byte
    int offset = inb(0x3D5) << 8; // Get the high byte
    port_byte_out(0x3D4, 15); // Now low byte
    offset += port_byte_in(0x3D5);
    return offset * 2; // Position * 2 (because each char uses 2 bytes)
}

void set_cursor_offset(int offset) {
    offset /= 2;
    outb(0x3D4, 14);
    port_byte_out(0x3D5, (uint8_t)(offset >> 8));
    port_byte_out(0x3D4, 15);
    port_byte_out(0x3D5, (uint8_t)(offset & 0xFF));
}

void print_backspace() {
    int offset = get_cursor_offset();

    if (offset >= 2) {
        offset -= 2;
        set_cursor_offset(offset);
        char* vidmem = (char*) VIDEO_ADDRESS;
        vidmem[offset] = ' ';              // Clear the character
        vidmem[offset + 1] = WHITE_ON_BLACK;
    }
}

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

void kprint(const char* str) {
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