#include "memfunc.h"

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