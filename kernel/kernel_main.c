#include "kernel.h"

__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
    0xe85250d6,    // magic number (Multiboot2)
    0,             // architecture (0 = i386)
    0,             // header length (filled by linker)
    -(0xe85250d6 + 0 + 0) // checksum
};

void kernel_main() {
    kernel_print("IronicOS Kernel Booted Successfully!\n");
    // Main loop of the kernel
    while (1) {
        // Kernel tasks would be executed here
    }
}