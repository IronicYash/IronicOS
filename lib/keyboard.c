#include "../lib/screen.h"
#include "../cpu/ports.h"
#include "../lib/keyboard.h"
#include "../lib/string.h"

#define MAX_INPUT 256

static char input_buffer[MAX_INPUT];
static int buffer_index = 0;

// Forward declarations
char scancode_to_ascii(unsigned char scancode);

void backspace() {
    if (buffer_index > 0) {
        buffer_index--;
        input_buffer[buffer_index] = '\0';
        print_backspace();  // remove last char from screen
    }
}

void clear_input_buffer() {
    for (int i = 0; i < MAX_INPUT; i++) {
        input_buffer[i] = '\0';
    }
    buffer_index = 0;
}

char* read_input() {
    clear_input_buffer();
    while (1) {
        asm volatile("hlt");  // Wait for input
        if (input_buffer[buffer_index - 1] == '\n') {
            input_buffer[buffer_index - 1] = '\0'; // Remove newline
            break;
        }
    }
    return input_buffer;
}

void keyboard_handler() {
    unsigned char scancode = port_byte_in(0x60);

    char key = scancode_to_ascii(scancode);
    if (key == 0) return;

    if (key == '\b') {
        backspace();
    } else if (key == '\n') {
        print_char('\n');
        input_buffer[buffer_index++] = '\n';
    } else {
        char str[2] = {key, '\0'};
        print(str);
        if (buffer_index < MAX_INPUT - 1) {
            input_buffer[buffer_index++] = key;
        }
    }
}

char scancode_to_ascii(unsigned char scancode) {
    static char kbd_US[128] = {
        0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
        'a','s','d','f','g','h','j','k','l',';','\'','`',  0, '\\',
        'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
        // rest are unused
    };
    return kbd_US[scancode];
}

void init_keyboard() {
    // clear_input_buffer();
    // Additional initialization if needed
    print("Keyboard initialized.\n");
}