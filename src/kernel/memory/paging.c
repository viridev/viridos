#include "paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

extern void load_page_dir(uintptr_t*);
extern void enable_paging();

void paging_init()
{
    for(uint32_t i = 0; i < 1024; i++)
    {
        page_directory[i] = 0b010;
    }

    for(uint32_t i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        first_page_table[i] = (i * 0x1000) | 0b011; // attributes: supervisor level, read/write, present.
    }

    // attributes: supervisor level, read/write, present
    page_directory[0] = (uintptr_t)first_page_table | 0b011;

    load_page_dir(page_directory);
    enable_paging();
}