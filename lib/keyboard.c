#include "keyboard.h"
#include "ports.h"
#include "screen.h"
#include "string.h"
#include <stdint.h>

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

/* Simple ring buffer */
#define KB_BUF_SIZE 128
static volatile char kb_buf[KB_BUF_SIZE];
static volatile int kb_head = 0, kb_tail = 0;

static const char scancode_to_ascii[128] = {
/* 0x00 - 0x0F */
0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', '\t',
/* 0x10 - 0x1F */
'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
/* 0x20 - 0x2F */
'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
/* 0x30 - 0x3F */
'b','n','m',',','.','/',0,'*',0,' ','0',0,0,0,0,0,
/* rest are zeros */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void kbd_handle_scancode(void) {
    uint8_t sc = inb(KBD_DATA_PORT);
    if (sc & 0x80) {
        /* key release - ignore for now */
        return;
    }
    char c = sc < 128 ? scancode_to_ascii[sc] : 0;
    if (!c) return;
    int next = (kb_head + 1) % KB_BUF_SIZE;
    if (next == kb_tail) {
        /* buffer full - drop */
        return;
    }
    kb_buf[kb_head] = c;
    kb_head = next;
}

void init_keyboard(void) {
    /* PIC remap + IDT entries must be set; the irq handler should call kbd_handle_scancode.
       We only ensure keyboard IRQ is enabled here (unmask IRQ1). */
    /* Unmask IRQ1 (keyboard) - assume PIC is set to remapped offsets */
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);
    outb(0x21, mask);
}

int keyboard_getchar(void) {
    if (kb_head == kb_tail) return 0;
    char c = kb_buf[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUF_SIZE;
    return (int)c;
}

int keyboard_getchar_blocking(void) {
    int c;
    while (!(c = keyboard_getchar())) { asm volatile("hlt"); }
    return c;
}