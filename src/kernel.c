#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <vga_text.h>
#include <io.h>
#include <ata.h>

void readfile(char *filename, char *buffer)
{
	char buf[511];

	ata_read_disk(0, buf);
	int lr = 0;

	int len = 0;
	char fn[255];

	bool found = false;

	for (int i = 0; i < 512; i++)
	{
		char v = buf[i];

		if (v == 0)
			break;

		if (v == '\1')
		{
			found = true;

			for (int j = 0; j < len; j++)
			{
				if (fn[j] != filename[j])
				{
					len = 0;
					found = false;

					break;
				}
			};

			for (int j = 0; j < len; j++)
			{
				buffer[j] = 0;
			}

			len = 0;
		}
		else if (v == '\2')
		{
			if (found)
				return;

			len = 0;
		}
		else
		{
			fn[len++] = v;
			buffer[len - 1] = v;
		}

		if (i == 511)
		{
			ata_read_disk(++lr, buf);
			i = 0;
		}
	}
}

void kernel_main(void)
{
	vgat_print("Boot succeeded, starting initialization...\n");
	vgat_print("Attempting to read from file...\n");

	char buf[511];

	ata_read_disk(0, buf);
	int lr = 0;

	for (int i = 0; i < 512; i++)
	{
		char v = buf[i];

		if (v != 0)
			vgat_putc(v);

		if (i == 511)
		{
			ata_read_disk(++lr, buf);
			i = 0;
		}
	}

	while (1)
		;
}