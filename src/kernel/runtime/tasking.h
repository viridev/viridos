#pragma once
#include <stdint.h>

enum TASK_TYPE
{
    KERNEL,
    DRIVER_1,
    DRIVER_2,
    USER
};

typedef struct regs
{
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp, esp, eip, eflags;
} regs_t;

typedef struct task
{
    regs_t registers;
    uint32_t page_dir;
    int type;

    struct task *next; // next task
} task_t;


void tasking_create_task(task_t *task, void (*main)(), uint32_t flags, uint32_t *page_dir);
void tasking_next();