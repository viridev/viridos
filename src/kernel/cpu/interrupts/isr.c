#include "isr.h"
#include <console.h>

static const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void page_fault(registers_t regs); // prototype

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    if (regs.int_no == 14)
        page_fault(regs);
    else
        console_log("Unhandled CPU exception ID=0x%x %s", regs.int_no, exception_messages[regs.int_no]);  

    asm volatile("hlt");
}

void page_fault(registers_t regs)
{
   // the faulting address is stored in the CR2 register.
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present = !(regs.err_code & 0x1);
   int read_write = regs.err_code & 0x2;           
   int user = regs.err_code & 0x4;
   int reserved = regs.err_code & 0x8;
   int id = regs.err_code & 0x10;

   console_log("PAGE FAULT OCCURRED at 0x%x\n", faulting_address);
   console_log("present: %d; r_w %d; user: %d; res: %d\n", present, read_write, user, reserved);
}