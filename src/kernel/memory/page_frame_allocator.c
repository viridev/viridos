#include "page_frame_allocator.h"
#include <memory/mem.h>
#include <multiboot.h>

size_t installed_memory = 0;
size_t reserved_memory = 0;
size_t usable_memory = 0;

size_t free_memory = 0;
size_t used_memory = 0;

uint8_t *map;
uint32_t map_size;

static void generate_map(); //prototype

void page_frame_allocator_init()
{
    if(!(mboot->flags >> 6 & 0x01))
        printf("Invalid memory map!");

    size_t usable_mem_len = 0;
    uint32_t usable_mem_start;

    for(int i = 0; i < mboot->mmap_length; i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t *mmmt = (multiboot_memory_map_t*)(mboot->mmap_addr + i);

        // get the biggest usable memory block
        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
            if (mmmt->len_low > usable_mem_len)
            {
                usable_mem_len = mmmt->len_low;
                usable_mem_start = mmmt->addr_low;
            }                
        printf("Start Addr: 0x%x%x | Length: 0x%x%x | Size: %x | Type: %d\n", mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);

        installed_memory += mmmt->len_low;
    }

    generate_map(usable_mem_start, usable_mem_len); // tell the generator how much memory to initally mark as being free
    lock_pages(&kernel_start, ((uint32_t)&kernel_end-(uint32_t)&kernel_start)/0x1000 + 1); // lock pages for the kernel code
    printf("MEMORY - kernel: start 0x%x, end 0x%x\n", &kernel_start, &kernel_end);
    printf("MEMORY: installed %dKB, reserved %dKB, usable %dKB\n", installed_memory/1024, reserved_memory/1024, usable_memory/1024);
    printf("MEMORY: free %dKB, used %dKB\n", free_memory/1024, used_memory/1024);
}

static void generate_map(uint32_t usable_mem_start, size_t len)
{
    map_size = installed_memory / 0x1000;
    map = kmalloc(map_size, 1, 0); // alloc space for the map

    // set the entire map as reserved
    for (int i = 0; i < map_size; i++)
        map[i] = PAGE_RESERVED;

    // free pages that fit the usable memory
    uint32_t index = usable_mem_start / 0x1000 + 1;
    uint32_t count = len / 0x1000 - 1;
    for (int i = 0; i < count; i++)
        map[i + index] = PAGE_FREE;

    // determine the amount of initial reserved and useable memory
    for (int i = 0; i < map_size; i++)
        if(map[i] == PAGE_RESERVED) reserved_memory += 4096;
        else if(map[i] == PAGE_FREE) usable_memory += 4096;
    free_memory = usable_memory;
}

void free_page(void *address)
{
    uint32_t index = (uint32_t)address / 0x1000;

    map[index] = PAGE_FREE;

    free_memory += 4096;
    used_memory -= 4096;
}
void free_pages(void *address, size_t count)
{
    uint32_t index = (uint32_t)address / 0x1000;

    for(int i = 0; i < count; i++) map[i+ + index] = PAGE_FREE;

    free_memory += count * 4096;
    used_memory -= count * 4096;
}

void lock_page(void *address)
{
    uint32_t index = (uint32_t)address / 0x1000;

    map[index] = PAGE_LOCKED;

    free_memory -= 4096;
    used_memory += 4096;
}
void lock_pages(void *address, size_t count)
{
    uint32_t index = (uint32_t)address / 0x1000;

    for(int i = 0; i < count; i++) map[i + index] = PAGE_LOCKED;

    free_memory -= count * 4096;
    used_memory += count * 4096;
}

void reserve_page(void *address)
{
    uint32_t index = (uint32_t)address / 0x1000;

    map[index] = PAGE_RESERVED;

    reserved_memory += 4096;
    usable_memory -= 4096;
}
void reserve_pages(void *address, size_t count)
{
    uint32_t index = (uint32_t)address / 0x1000;

    for(int i = 0; i < count; i++) map[i + index] = PAGE_RESERVED;

    reserved_memory += count * 4096;
    usable_memory -= count * 4096;
}

void* request_page()
{
    for (int i = 0; i < map_size; i++)
        if (map[i] == PAGE_FREE)
        {
            lock_page((void*)(i * 0x1000));
            return (void*)(i * 4096);
        }    

    return NULL;
    // out of ram
}

void get_ram_size() { return installed_memory;}
void get_free_ram() { return free_memory;}
void get_used_ram() { return used_memory;}