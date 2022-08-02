SOURCES=$(shell find ./kernel -name '*.c')

CC=i686-elf-gcc
CFLAGS=-Ikernel/ -Isystem/ -Iutils/ -I./ -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers
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


link:
	$(LD) -w -T 'linker.ld' -o 'virid.bin' -ffreestanding -O2 -nostdlib $(LINK_SOURCES) -lgcc	

iso:
	rm -rf isodir/
	mkdir -p isodir/boot/grub
	cp virid.bin isodir/boot/
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o virid.iso isodir

clean:
	rm -rf isodir/
	rm -rf src/*.a
	rm -rf src/*.o
	rm -rf *.iso
	rm -rf *.bin

run:
	qemu-system-i386 -cdrom virid.iso -serial stdio -vga std -no-reboot -no-shutdown -m 128M