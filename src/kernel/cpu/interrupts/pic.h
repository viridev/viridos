#pragma once
#include <stddef.h>

#define PIC1_CONTROL 0x20
#define PIC1_DATA 0x21
#define PIC2_CONTROL 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

void pic_remap();