// lib/shell.c - IronicOS simple production shell
#include "shell.h"
#include "serial.h"
#include "string.h"

#define MAX_INPUT 128

void shell_init() {
    serial_puts("[IronicOS] Shell initialized.\n");
}

void shell_run() {
    char input[MAX_INPUT];
    int pos = 0;

    serial_puts("[IronicOS] Entering shell. Type 'help' for commands.\n");

    while (1) {
        serial_puts("> ");  // prompt
        pos = 0;
        memset(input, 0, MAX_INPUT);

        // Read user input
        while (1) {
            char c = serial_getc();

            // Handle Enter
            if (c == '\r' || c == '\n') {
                serial_puts("\n");
                break;
            }

            // Handle Backspace
            if (c == 0x08 && pos > 0) {  // backspace ASCII
                pos--;
                serial_puts("\b \b");
                continue;
            }

            if (pos < MAX_INPUT - 1) {
                input[pos++] = c;
                char s[2] = {c, 0};
                serial_puts(s);  // echo character
            }
        }

        // Remove trailing newline
        input[pos] = '\0';

        // Parse commands
        if (strcmp(input, "help") == 0) {
            serial_puts("Available commands: help, echo, clear, reboot\n");
        } else if (strncmp(input, "echo ", 5) == 0) {
            serial_puts(input + 5);
            serial_puts("\n");
        } else if (strcmp(input, "clear") == 0) {
            serial_puts("\033[2J\033[H"); // ANSI clear screen
        } else if (strcmp(input, "reboot") == 0) {
            serial_puts("Rebooting...\n");
            __asm__ volatile ("int $0x19"); // BIOS reboot
        } else if (strlen(input) == 0) {
            continue;  // ignore empty input
        } else {
            serial_puts("Unknown command: ");
            serial_puts(input);
            serial_puts("\n");
        }
    }
}
