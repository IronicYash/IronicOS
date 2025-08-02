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

void run_shell() {
    print("Welcome to IronicOS Shell!\n");
    print("Type 'help' to get started.\n\n");

    while (1) {
        print("> ");
        char* input = read_input();

        if (strcmp(input, "help") == 0) {
            print("Available commands:\n");
            print("  help   - Show this help message\n");
            print("  clear  - Clear the screen\n");
            print("  about  - About this OS\n");
            print("  reboot - Simulate reboot\n");
        }
        else if (strcmp(input, "clear") == 0) {
            clear_screen();
        }
        else if (strcmp(input, "about") == 0) {
            print("IronicOS - a hobby 32-bit OS built from scratch by Yash.\n");
        }
        else if (strcmp(input, "reboot") == 0) {
            print("Rebooting... (simulated)\n");
        }
        else {
            print("Unknown command. Type 'help'.\n");
        }
    }
}

void kernel_main() {
    clear_screen();
    print("IronicOS Kernel Booted Successfully!\n");
    init_memory();
    init_keyboard();
    run_shell();
    
    // This point should never be reached in a real OS
    while (1) {
        // Halt the CPU
        asm volatile ("hlt");
    }
}