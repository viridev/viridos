; global methods
[GLOBAL idt_flush]

; external methods from c
[EXTERN isr_handler]
[EXTERN irq_handler]
[EXTERN pit_handler]


idt_flush:
    mov EAX, [ESP+4]    ; load table pointer into eax
    lidt [EAX]          ; load table

    mov AX, 0x10        ; load segment value into ax
    mov DS, AX          ; load segment into ds
    mov ES, AX          ; load segment into es
    mov FS, AX          ; load segment into fs
    mov GS, AX          ; load segment into gs
    mov SS, AX          ; load segment into ss

    ; finished flushing - far jump return
    jmp 0x08:.idt_flush_end
.idt_flush_end:
    ret

global isr_common_stub:function isr_common_stub.end-isr_common_stub
isr_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop eax        ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret  
.end:

global irq_common_stub:function irq_common_stub.end-irq_common_stub
irq_common_stub:
    pushad              ; push general purpose registers

    mov AX, DS          ; load data segment into ax
    push EAX            ; push data segment stored in ax

    mov AX, 0x10        ; load segment value into ax
    mov DS, AX          ; load segment into ds
    mov ES, AX          ; load segment into es
    mov FS, AX          ; load segment into fs
    mov GS, AX          ; load segment into gs
    mov SS, AX          ; load segment into ss

    push ESP            ; push register struct pointer as argument
    call irq_handler    ; call isr handler method
    add ESP, 4          ; pop register struct

    pop EBX             ; pop segment register
    mov DS, BX          ; restore ds segment
    mov ES, BX          ; restore es segment
    mov FS, BX          ; restore fs segment
    mov GS, BX          ; restore gs segment
    mov SS, BX          ; restore ss segment

    popad               ; pop general purpose registers
    add ESP, 8          ; pop error code and interrupt number
    iretd               ; return from interrupt
.end:

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
global irq_syscall

; 0: Divide By Zero Exception
isr0:
    push dword 0
    push dword 0
    jmp isr_common_stub

; 1: Debug Exception
isr1:
    push dword 0
    push dword 1
    jmp isr_common_stub

; 2: Non Maskable Interrupt Exception
isr2:
    push dword 0
    push dword 2
    jmp isr_common_stub

; 3: Int 3 Exception
isr3:
    push dword 0
    push dword 3
    jmp isr_common_stub

; 4: INTO Exception
isr4:
    push dword 0
    push dword 4
    jmp isr_common_stub

; 5: Out of Bounds Exception
isr5:
    push dword 0
    push dword 5
    jmp isr_common_stub

; 6: Invalid Opcode Exception
isr6:
    push dword 0
    push dword 6
    jmp isr_common_stub

; 7: Coprocessor Not Available Exception
isr7:
    push dword 0
    push dword 7
    jmp isr_common_stub

; 8: Double Fault Exception (With Error Code!)
isr8:
    push dword 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun Exception
isr9:
    push dword 0
    push dword 9
    jmp isr_common_stub

; 10: Bad TSS Exception (With Error Code!)
isr10:
    push dword 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    push dword 11
    jmp isr_common_stub

; 12: Stack Fault Exception (With Error Code!)
isr12:
    push dword 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    push dword 13
    jmp isr_common_stub

; 14: Page Fault Exception (With Error Code!)
isr14:
    push dword 14
    jmp isr_common_stub

; 15: Reserved Exception
isr15:
    push dword 0
    push dword 15
    jmp isr_common_stub

; 16: Floating Point Exception
isr16:
    push dword 0
    push dword 16
    jmp isr_common_stub

; 17: Alignment Check Exception
isr17:
    push dword 17
    jmp isr_common_stub

; 18: Machine Check Exception
isr18:
    push dword 0
    push dword 18
    jmp isr_common_stub

; 19: Reserved
isr19:
    push dword 0
    push dword 19
    jmp isr_common_stub

; 20: Reserved
isr20:
    push dword 0
    push dword 20
    jmp isr_common_stub

; 21: Reserved
isr21:
    push dword 21
    jmp isr_common_stub

; 22: Reserved
isr22:
    push dword 0
    push dword 22
    jmp isr_common_stub

; 23: Reserved
isr23:
    push dword 0
    push dword 23
    jmp isr_common_stub

; 24: Reserved
isr24:
    push dword 0
    push dword 24
    jmp isr_common_stub

; 25: Reserved
isr25:
    push dword 0
    push dword 25
    jmp isr_common_stub

; 26: Reserved
isr26:
    push dword 0
    push dword 26
    jmp isr_common_stub

; 27: Reserved
isr27:
    push dword 0
    push dword 27
    jmp isr_common_stub

; 28: Reserved
isr28:
    push dword 0
    push dword 28
    jmp isr_common_stub

; 29: Reserved
isr29:
    push dword 0
    push dword 29
    jmp isr_common_stub

; 30: Reserved
isr30:
    push dword 0
    push dword 30
    jmp isr_common_stub

; 31: Reserved
isr31:
    push dword 0
    push dword 31
    jmp isr_common_stub

; 32: PIT - used for thread switching
irq0:
    push dword 0
    push dword 32
	jmp irq_common_stub

; 33: PS/2 Keyboard
irq1:
	push dword 1
	push dword 33
	jmp irq_common_stub

irq2:
	push dword 2
	push dword 34
	jmp irq_common_stub

irq3:
	push dword 3
	push dword 35
	jmp irq_common_stub

irq4:
	push dword 4
	push dword 36
	jmp irq_common_stub

irq5:
	push dword 5
	push dword 37
	jmp irq_common_stub

irq6:
	push dword 6
	push dword 38
	jmp irq_common_stub

irq7:
	push dword 7
	push dword 39
	jmp irq_common_stub

; RTC
irq8:
	push dword 8
	push dword 40
	jmp irq_common_stub

irq9:
	push dword 9
	push dword 41
	jmp irq_common_stub

irq10:
	push dword 10
	push dword 42
	jmp irq_common_stub

irq11:
	push dword 11
	push dword 43
	jmp irq_common_stub

irq12:
	push dword 12
	push dword 44
	jmp irq_common_stub

irq13:
	push dword 13
	push dword 45
	jmp irq_common_stub

irq14:
	push dword 14
	push dword 46
	jmp irq_common_stub

irq15:
	push dword 15
	push dword 47
	jmp irq_common_stub