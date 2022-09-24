#include "pit.h"
#include <console.h>
#include <cpu/interrupts/irq.h>
#include <cpu/io.h>

int pit_freq;
uint64_t elapsed_time = 0;

static void pit_set_freq(int hz)
{
    pit_freq = hz;
    int divisor = BASE_FREQ / hz; /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void pit_handler(struct regs *r)
{
    elapsed_time += 1000 / pit_freq;
}

void pit_init(int hz)
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, pit_handler);
    pit_set_freq(hz);

    asm("sti"); // set the interrupt flag so the IRQs actually get called
    console_log("PIT initialized with a freq of %dhz.\n", hz);
}

uint64_t pit_get_elapsed_time()
{
    return elapsed_time;
}