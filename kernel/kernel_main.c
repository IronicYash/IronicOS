#include "kernel.h"

void kernel_main() {
    char *video = (char*)0xb8000;
    const char *msg = "Hello, IronicOS!";
    for (int i = 0; msg[i] != 0; i++) {
        video[i * 2] = msg[i];
        video[i * 2 + 1] = 0x0F; // White on black
    }
    // Main loop of the kernel
    while (1) {
        // Kernel tasks would be executed here
    }
}