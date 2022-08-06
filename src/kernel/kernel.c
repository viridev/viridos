#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <video/vga_text.h>
#include <video/vbe.h>

#include <cpu/descriptors/gdt.h>
#include <cpu/descriptors/idt.h>

#include <memory/mm.h>

void kernel_main(multiboot_t *mbd)
{
	mboot = mbd;

	console_init();
	gdt_init();
	idt_init();

	mm_init();

	while (1)
		;
}