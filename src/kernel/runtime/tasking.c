#include "tasking.h"
#include <video/vga_text.h>
#include <memory/page_frame_allocator.h>
#include <memory/paging.h>

static task_t *running_task;
static task_t *last_task;

void tasking_create_task(task_t *task, void (*main)(), uint32_t flags, uint32_t *page_dir)
{
    task->registers.eax = 0;
    task->registers.ebx = 0;
    task->registers.ecx = 0;
    task->registers.edx = 0;
    task->registers.esi = 0;
    task->registers.edi = 0;
    task->registers.eflags = flags;
    task->registers.eip = (uint32_t) main;
    task->registers.esp = (uint32_t) request_page() + 0x1000;

	//task->registers.cr3 = (uint32_t) page_dir;
    task->next = 0;
}

void tasking_next()
{
    asm volatile("push %eax");
	asm volatile("push %ebx");
	asm volatile("push %ecx");
	asm volatile("push %edx");
	asm volatile("push %esi");
	asm volatile("push %edi");
	asm volatile("push %ebp");

	asm volatile("mov %%esp, %%eax":"=a"(running_task->registers.esp));
	running_task = running_task->next;
	//asm volatile("mov %%eax, %%cr3": :"a"(running_task->registers.cr3));
	asm volatile("mov %%eax, %%esp": :"a"(running_task->registers.esp));

	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");

	asm volatile("jmp *%0" : : "r" (running_task->registers.eip));
}