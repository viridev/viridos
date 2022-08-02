#include <stddef.h>
#include <stdint.h>

#include <video/vga_text.h>

#include <cpu/descriptors/gdt.h>
#include <cpu/descriptors/idt.h>

void kernel_main(void)
{
	console_init();
	gdt_init();
	idt_init();

	

	asm("int $0x80");

	while (1)
		;
}