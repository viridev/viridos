#include "mm.h"
#include <multiboot.h>
#include <realmode.h>
#include <video/vga_text.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t installed_memory;

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

static void detect_installed_ram()
{
    installed_memory = 0;

    if(!(mboot->flags >> 6 & 0x01))
        printf("Invalid memory map!");

    int i;
    for(i = 0; i < mboot->mmap_length; 
        i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t *mmmt = (multiboot_memory_map_t*)(mboot->mmap_addr + i);
        //printf("Start Addr: %x%x | Length: %x%x | Size: %x | Type: %d\n", mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);

        installed_memory += mmmt->len_low;
    }
    printf("Installed Memory: %x(%dMB)", installed_memory, installed_memory/1024/1024);
}

void mm_init()
{
    detect_installed_ram();
}