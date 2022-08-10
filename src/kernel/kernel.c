#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <video/vga_text.h>
#include <video/vbe.h>

#include <cpu/descriptors/gdt.h>
#include <cpu/descriptors/idt.h>
#include <cpu/pit.h>

#include <memory/mem.h>
#include <memory/page_frame_allocator.h>
#include <memory/paging.h>

static void testMain()
{
    printf("Msg from thread!");
}

void kernel_main(multiboot_t *mbd)
{
	mboot = mbd;

	console_init();
	gdt_init();
	idt_init();

	mem_init();
	page_frame_allocator_init();
	paging_init();
	
	//pit_init(69420);

	while (1)
	asm volatile ("hlt");
		;
}