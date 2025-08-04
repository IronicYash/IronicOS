#include "keyboard.h"
#include "shell.h"
#include "screen.h"
#include "io.h"
#include "../cpu/irq.h"

#define KEYBOARD_DATA_PORT 0x60

// US QWERTY scancode-to-ASCII table (no shift handling for now)
static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', /* backspace */
    '\t', /* tab */
    'q','w','e','r','t','y','u','i','o','p','[',']','\n', /* enter */
    0, /* control */
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, /* left shift */
    '\\','z','x','c','v','b','n','m',',','.','/',
    0, /* right shift */
    '*',
    0, /* alt */
    ' ', /* space bar */
    0, /* caps lock */
    0, /* F1 */
    0, /* F2 */
    0, /* F3 */
    0, /* F4 */
    0, /* F5 */
    0, /* F6 */
    0, /* F7 */
    0, /* F8 */
    0, /* F9 */
    0, /* F10 */
    0, /* num lock */
    0, /* scroll lock */
    0, /* home */
    0, /* up arrow */
    0, /* page up */
    '-',
    0, /* left arrow */
    0,
    0, /* right arrow */
    '+',
    0, /* end */
    0, /* down arrow */
    0, /* page down */
    0, /* insert */
    0, /* delete */
    0, 0, 0, 0,
    0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0
};

static void keyboard_callback() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode > 127) return; // ignore key release (for now)

    char ascii = scancode_to_ascii[scancode];
    if (ascii) {
        shell_handle_input(ascii); // forward to shell
    }

    outb(0x20, 0x20); // send EOI to PIC
}

void keyboard_init() {
    irq_install_handler(1, keyboard_callback); // IRQ1 = PS/2 keyboard
}