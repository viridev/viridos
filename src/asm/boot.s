.global _start

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
; .set VIDINFO,  1<<2
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

	# aout kludge (unused)
    .long 0,0,0,0,0

    # Video mode
    .long   0       # Linear graphics please?
    .long   0       # Preferred width
    .long   0       # Preferred height
    .long   32      # Preferred pixel depth

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
_start:
	mov $stack_top, %esp

	push %ebx # push pointer to the Multiboot information structure

	call kernel_main

	cli

.size _start, . - _start
