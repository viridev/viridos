#include "irq.h"
#include "pic.h"

isr_t irq_handlers[16];

void irq_handler(registers_t *regs)
{  
    void (*handler)(registers_t *regs);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_handlers[regs->int_no - 32] ;
    if (handler)
    {
        handler(regs);
    }

    // send slave end of interrupt if necessary
    if (regs->int_no >= 40)
        outb(PIC2_CONTROL, PIC_EOI);

    // send master eio
    outb(PIC1_CONTROL, PIC_EOI);
}

void irq_install_handler(uint8_t n, isr_t handler)
{
    irq_handlers[n] = handler;
}