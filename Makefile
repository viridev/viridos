AS = i686-elf-as
CC = i686-elf-gcc

C_SOURCES = $(wildcard src/*.c)
ASM_SOURCES = $(wildcard src/*.s)

OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.s=.o}

all: os.iso

%.o: %.c
	${CC} -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude

%.o: %.s
	${AS} $< -o $@

os.bin: ${OBJ}
	${CC} -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

check-multiboot: os.bin
	grub-file --is-x86-multiboot os.bin

os.iso: check-multiboot
	rm -rf isodir/
	mkdir -p isodir/boot/grub
	cp os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

clean:
	rm -rf isodir/
	rm -rf src/*.o
