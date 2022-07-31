#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <vga_text.h>
#include <io.h>
#include <ata.h>

void kernel_main(void)
{
	vgat_bg = 0x1;
	vgat_fill(vgat_bg);
	vgat_rect(0, 24, 80, 1, 0x7);
	vgat_print("Boot succeeded, starting initialization...\n");
	vgat_print("Attempting to read from drive using ATAPI...\n\"");

	char buffer[511];

	ata_read_disk(0, buffer);
	int block = 0;

	for (int i = 0; i < 512; i++)
	{
		char c = buffer[i];

		if (c == '\0')
			break;

		vgat_putc(c);

		if (i == 511)
		{
			ata_read_disk(++block, buffer);
			i = 0;
		}
	}

	vgat_print("\"\nSuccess!\n");

	while (1)
		;
}