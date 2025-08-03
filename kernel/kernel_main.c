#include "kernel.h"
#include "../lib/screen.h"
#include "../lib/keyboard.h"
#include "memory.h"
#include "../lib/string.h"

__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
    0xe85250d6,    // magic number (Multiboot2)
    0,             // architecture (0 = i386)
    0,             // header length (filled by linker)
    -(0xe85250d6 + 0 + 0) // checksum
};

void kernel_main() {
    clear_screen();
    print("IronicOS Kernel Booted Successfully!\n");
    init_memory();
    init_keyboard();
    run_shell();
    
    // This point should never be reached in a real OS
    while (1) {
        __asm__ __volatile__("hlt");
    }
}