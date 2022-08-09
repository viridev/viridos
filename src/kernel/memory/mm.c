#include "mm.h"
#include <multiboot.h>
#include <realmode.h>
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
    printf("Installed Memory: %x(%dMB)\n", installed_memory, installed_memory/1024/1024);
    printf("Kernel start:%x Kernel end:%x\n", &kernel_start, &kernel_end);
}

void mm_init()
{
    detect_installed_ram();
    placement_adr = &kernel_end;
}

void* kmalloc(size_t size, int align, uint32_t *phys)
{
    if (align)
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