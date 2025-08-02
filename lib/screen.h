#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

void clear_screen();
void print(const char* str);
void print_char(char c);
void new_line();

#endif