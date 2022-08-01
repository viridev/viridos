#include "pic.h"
#include <cpu/io.h>


void pic_remap()
{
  outb(PIC1_CONTROL, 0x11);
  outb(PIC2_CONTROL, 0x11);

  outb(PIC1_DATA, 0x20);
  outb(PIC2_DATA, 0x28);
  /* Tell the master that he has a slave */
  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);
  /* Enabled 8086 mode */
  outb(PIC1_DATA, 0x01);
  outb(PIC2_DATA, 0x01);

  // only enable keyboard IRQ
  outb(PIC1_DATA, 0x00);
  outb(PIC2_DATA, 0x00);
}