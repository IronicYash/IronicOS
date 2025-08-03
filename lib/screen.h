#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F


#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

int get_cursor_offset();
void set_cursor_offset(int offset);
void clear_screen();
void kprint(const char* str);
void print_char(char c);
void new_line();
void print_backspace(); 

#endif