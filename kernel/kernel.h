#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F

uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
uint16_t cursor = 0;

// void kernel_print(const char* str) {
//     for (int i = 0; str[i] != '\0'; i++) {
//         vga_buffer[cursor++] = (WHITE_ON_BLACK << 8) | str[i];
//     }
// }

// Data structure definitions
typedef struct {
    int id;
    char name[32];
} process_t;

// Constants
#define MAX_PROCESSES 256

#endif // KERNEL_H