#include "mem.h"
#include <video/vga_text.h>

uint32_t placement_adr;

void* memset(void* dest, int c, uint32_t n)
{
    uint32_t len = n / 4;
    asm volatile ("cld; rep stosl" : "+c" (len), "+D" (dest) : "a" (c) : "memory");
    return dest;
}   
void* memcpy(void* dest, const void* src, uint32_t n)
{
    asm volatile ("cld; rep movsb" : "+c" (n), "+S" (src), "+D" (dest) :: "memory");
    return dest;
}

void* memmove(void* dest, const void* src, uint32_t n)
{
    return memcpy(dest, src, n);
}

void mem_init()
{
    placement_adr = &kernel_end;
}

void* kmalloc(size_t size, int align, uint32_t *phys)
{
    if (align && (placement_adr % 0x1000 != 0))
    {
        placement_adr &= 0xFFFFF000;
        placement_adr += 0x1000;
    }
    if (phys)
    {
        *phys = placement_adr;
    }

    uint32_t temp = placement_adr;
    placement_adr += size;
    return temp;
}