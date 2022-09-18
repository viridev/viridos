#include "ps2_kb.h"
#include <console.h>
#include <cpu/io.h>

int shift = 0;
const char *keys = "  1234567890-=\b\tqwertyuiop[]\n  asdfghjkl;'` \\zxcvbnm,./";
const char *keys_shift = "  !@#$%^&*()_+\b\tQWERTYUIOP{}\n  ASDFGHJKL:\"~ |ZXCVBNM<>?";

char kb_code_to_ascii(uint8_t key)
{
    switch (key)
    {
    case 0x39:
        return ' ';
        break;
    default:
        if (key >= 0x2 && key <= 0x35)
        {
            char c;
            if (shift)
                c = keys_shift[key];
            else
                c = keys[key];
            if (c != ' ') return c;
        }            
        break;
    }

    return '\0';
}

uint8_t last_key;
uint8_t key_map[0x58];
void kb_handler(struct regs *r)
{
    uint8_t code = inb(0x60);
    last_key = code;

    switch (code)
    {
    case 0x2A: // left shift pressed
        shift = 1;
        break;
    case 0xAA: // left shift released
        shift = 0;
        break;
    }

    if (code > 0x80) // key release
        key_map[code - 0x80] = 0;
    else
        key_map[code] = 1;
}

uint8_t *ps2_kb_get_key_map()
{
    return &key_map;
}

uint8_t ps2_kb_get_last_key()
{
    return last_key;
}

void ps2_kb_init()
{
    // Installs 'timer_handler' to IRQ1
    irq_install_handler(1, kb_handler);

    asm("sti"); // set the interrupt flag so the IRQs actually get called
}