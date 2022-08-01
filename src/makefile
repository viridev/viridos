SOURCES=$(shell find ./kernel -name '*.c')


CC=i686-elf-gcc
CFLAGS=-Ikernel/ -Isystem/ -Iutils/ -I./ -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers
C_FILES_OUT = $(SOURCES:.c=.o)

LD=i686-elf-gcc
AS=i686-elf-as
NASM=nasm
ASFLAGS=-felf32

LINK_SOURCES=$(shell find . -name '*.o')

all: as $(C_FILES_OUT) link run-kernel

# Kernel

as:
	$(AS) 'asm/boot.s' -o 'asm/boot.o'
	$(NASM) $(ASFLAGS) 'asm/gdt.asm' -o 'asm/gdt.o'
	$(NASM) $(ASFLAGS) 'asm/interrupts.asm' -o 'asm/interrupts.o'

# Link

link:
	$(LD) -w -T 'linker.ld' -o 'kernel.bin' -ffreestanding -O2 -nostdlib $(LINK_SOURCES) -lgcc	

clean:
	rm -rf isodir/
	rm -rf src/*.a
	rm -rf src/*.o
	rm -rf *.iso
	rm -rf *.bin

# Kernel running

run-kernel:
	qemu-system-i386 -kernel kernel.bin -serial stdio -vga std -no-reboot -no-shutdown -m 128M
#	qemu-system-i386 -kernel kernel.bin -drive id=disk,file=myimg.img,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0 -serial stdio -vga std -no-reboot -no-shutdown -m 128M