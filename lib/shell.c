#include "shell.h"
#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "../cpu/irq.h"
#include <stdint.h>

static char line_buf[128];

static void exec_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        printf("Built-in: help, clear, echo, reboot\n");
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        printf("%s\n", cmd + 5);
    } else if (strcmp(cmd, "reboot") == 0) {
        /* Triple fault / jump to reset vector - crude */
        asm volatile ("cli");
        for (;;) asm volatile ("hlt");
    } else {
        printf("Unknown command: %s\n", cmd);
    }
}

void shell_loop(void) {
    printf("> ");
    int pos = 0;
    while (1) {
        int c = keyboard_getchar();
        if (!c) { asm volatile("hlt"); continue; }
        if (c == '\r' || c == '\n') {
            putchar('\n');
            line_buf[pos] = '\0';
            if (pos > 0) exec_command(line_buf);
            pos = 0;
            printf("> ");
        } else if (c == '\b') {
            if (pos > 0) { pos--; printf("\b \b"); }
        } else {
            if (pos < (int)(sizeof(line_buf) - 1)) {
                line_buf[pos++] = (char)c;
                putchar((char)c);
            }
        }
    }
}