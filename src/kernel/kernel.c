#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <console.h>
#include <video/vbe.h>

#include <cpu/descriptors/gdt.h>
#include <cpu/descriptors/idt.h>

#include <memory/mem.h>
#include <memory/paging.h>

#include <cpu/pit.h>

#include <other/pci.h>

void kernel_main(multiboot_t *mbd)
{
	mboot = mbd;

	console_init();
	gdt_init();
	idt_init();

	mem_init();
	page_frame_allocator_init();
	// paging_init();

	pci_enumerate();

	pit_init(500);

	while (1)
		asm volatile ("hlt");
}