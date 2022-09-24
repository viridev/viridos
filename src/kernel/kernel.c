#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <console.h>
#include <video/vbe.h>

#include <cpu/descriptors/gdt.h>
#include <cpu/descriptors/idt.h>

#include <memory/mem.h>
#include <memory/paging.h>

#include <devices/pci.h>

#include <devices/chips/pit.h>
#include <devices/drivers/ps2/ps2_kb.h>

#include <kshell.h>

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
	pit_init(200);

	ps2_kb_init();
	
	console_log("Press any key to continue...");
	printf(">");
	console_wait_for_key();
	kshell_start();

	while (1)
		asm volatile ("hlt");
}