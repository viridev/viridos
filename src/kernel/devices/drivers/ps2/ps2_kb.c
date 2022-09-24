#include "ps2_kb.h"
#include <console.h>
#include <cpu/io.h>

int shift = 0;

uint8_t last_key;
uint8_t key_map[0x58];
void kb_handler(struct regs *r)
{
    uint8_t code = inb(0x60);

    if (code == 0x2A)
        shift = 1;     
    else if (code == 0xAA)
        shift = 0;
    else if (code > 0x80) // key release
    {
        key_map[code - 0x80] = 0;
        if (code - 0x80 == last_key) last_key = 0;
    }
    else
    {
        key_map[code] = 1;
        last_key = code;
    }        

    console_on_key_down();
}

void ps2_kb_get_key_map(uint8_t *dest)
{
    for (int i = 0; i < 0x58; i++)
        dest[i] = key_map[i];    
}

uint8_t ps2_kb_get_last_key()
{
    return last_key;
}

int ps2_kb_is_shifting()
{
    return shift;
}

void ps2_kb_init()
{
    // Installs 'timer_handler' to IRQ1
    irq_install_handler(1, kb_handler);

    asm("sti"); // set the interrupt flag so the IRQs actually get called
}