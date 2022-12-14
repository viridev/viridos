#include "vbe.h"
#include <console.h>
#include <realmode.h>
#include <memory/mem.h>

vbe_info_t *vib;
vbe_mode_info_t *mib;

vbe_info_t *vbe_get_info()
{
	vbe_info_t *info;
	registers16_t regs;

	info = (struct vbe_info*)0x80000;

	memcpy(info->signature, "VBE2", 4);

	memset(&regs, 0, sizeof regs);
	regs.ES = (uint32_t)info >> 4;
	regs.AX = 0x4f00;
	_int_32(0x10, &regs);

	if((regs.AX & 0xffff) != 0x4f) {
		return 0;
	}

	return info;
}

vbe_mode_info_t *vbe_get_mode_info(int mode)
{
	vbe_mode_info_t *mi;
	registers16_t regs;

	mi = (struct vbe_mode_info*)(0x80000 + 512);

	memset(&regs, 0, sizeof regs);
	regs.ES = (uint32_t)mi >> 4;
	regs.AX = 0x4f01;
	regs.CX = mode;
	_int_32(0x10, &regs);

	if((regs.AX & 0xffff) != 0x4f) {
		return 0;
	}

	return mi;
}

int vbe_set_mode(int mode)
{
	registers16_t regs;

	memset(&regs, 0, sizeof regs);
	regs.AX = 0x4f02;
	regs.BX = mode;
	_int_32(0x10, &regs);

	if(regs.AX == 0x100) {
		return -1;
	}
	return 0;
}

void vbe_init(int width, int height, int bpp)
{
	vib = vbe_get_info();
	if (vib == 0)
	{
		console_error("VBE: could not retrieve vib info.");
		return;
	}

    uint16_t *modes = (uint16_t *)(vib->video_modes);

    for(int i = 0; modes[i] != 0xFFFF; i++)
    {
        vbe_mode_info_t *mode = vbe_get_mode_info(i);

        if(mode->width == width && mode->height == height && mode->bpp == bpp)
		{
			console_log("Video mode found!");
			vbe_current_mode = mode;
			if(!vbe_set_mode(&modes[i]))
			{
				console_error("VBE: could not set video mode.");
				vbe_initialized = 1;
			}
			else
				console_log("Video mode set!");
			
			return;
		}
    }
	console_log("No corresponding video mode found! (%d, %d, %d)", width, height, bpp);
}