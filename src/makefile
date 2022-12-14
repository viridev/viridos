SOURCES=$(shell find ./kernel -name '*.c')

CC=i686-elf-gcc
CFLAGS=-Ikernel/ -I./ -fno-use-cxa-atexit -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers
C_FILES_OUT = $(SOURCES:.c=.o)

LD=i686-elf-gcc
AS=i686-elf-as
NASM=nasm
ASFLAGS=-felf32

LINK_SOURCES=$(shell find . -name '*.o')

all: as $(C_FILES_OUT) link iso run

as:
	$(AS) 'asm/boot.s' -o 'asm/boot.o'
	$(NASM) $(ASFLAGS) 'asm/gdt.asm' -o 'asm/gdt.o'
	$(NASM) $(ASFLAGS) 'asm/interrupts.asm' -o 'asm/interrupts.o'
	$(NASM) $(ASFLAGS) 'asm/realmode.asm' -o 'asm/realmode.o'
	$(NASM) $(ASFLAGS) 'asm/paging.asm' -o 'asm/paging.o'

link:
	$(LD) -w -T 'linker.ld' -o 'virid.bin' -ffreestanding -O2 -nostdlib $(LINK_SOURCES) -lgcc	

iso:
	rm -rf isodir/
	mkdir -p isodir/boot/grub
	cp virid.bin isodir/boot/
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o virid.iso isodir

clean:
	rm -rf $(shell find . -name '*.o') $(shell find . -name '*.a') $(shell find . -name '*.bin') $(shell find . -name '*.iso')

run:
	qemu-system-x86_64 -cdrom virid.iso -serial stdio -vga std -no-reboot -no-shutdown -m 1G -drive id=disk,file=myimg.img,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0