#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../cpu/irq.h"
#include "screen.h"
#include "string.h"

static char scancode_to_char[] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_callback(registers_t* regs) {
    uint8_t scancode = inb(0x60);

    if (scancode < sizeof(scancode_to_char)) {
        char key = scancode_to_char[scancode];
        if (key) {
            char str[2] = { key, '\0' };
            kprint(str);
        }
    }
    (void)regs;  // Avoid unused warning
}

void init_keyboard() {
    irq_register_handler(1, keyboard_callback);
}