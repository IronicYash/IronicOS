#include "shell.h"
#include "screen.h"
#include "keyboard.h"
#include "../cpu/irq.h"
#include "../cpu/isr.h"
#include "../lib/string.h"

#define MAX_INPUT_LENGTH 128

static char input_buffer[MAX_INPUT_LENGTH];
static int input_length = 0;

void shell_init() {
    clear_screen();
    print("Welcome to IronicOS Shell\n");
    print("Type 'help' for a list of commands.\n\n");
    shell_prompt();
}

void shell_prompt() {
    print("\n> ");
}

void shell_handle_input(char c) {
    if (c == '\n') {
        input_buffer[input_length] = '\0';
        print("\n");
        shell_execute(input_buffer);
        input_length = 0;
        shell_prompt();
    } else if (c == '\b' || c == 127) { // Handle backspace
        if (input_length > 0) {
            input_length--;
            print("\b \b"); // erase character on screen
        }
    } else if (input_length < MAX_INPUT_LENGTH - 1) {
        input_buffer[input_length++] = c;
        print_char(c);
    }
}

void shell_execute(const char* input) {
    if (strcmp(input, "help") == 0) {
        shell_cmd_help();
    } else if (strcmp(input, "clear") == 0) {
        clear_screen();
    } else if (strncmp(input, "echo ", 5) == 0) {
        print(input + 5);
        print("\n");
    } else if (strcmp(input, "halt") == 0) {
        print("System halted.\n");
        __asm__ volatile ("hlt");
    } else {
        print("Unknown command. Type 'help' for a list.\n");
    }
}

void shell_cmd_help() {
    print("Available commands:\n");
    print("  help   - Show this help message\n");
    print("  clear  - Clear the screen\n");
    print("  echo   - Echo the text\n");
    print("  halt   - Halt the system\n");
}