#include "paging.h"
#include <memory/mem.h>
#include <memory/page_frame_allocator.h>
#include <video/vga_text.h>

page_dir_t *currentd_dir = 0;

extern void load_page_dir(uintptr_t*);
extern void enable_paging();

// kernel dir
page_table_t *first_page_table;

void paging_init()
{
    kernel_dir = (page_dir_t*)kmalloc(sizeof(page_dir_t), 1, 0);
    first_page_table = (page_table_t*)kmalloc(sizeof(page_table_t), 1, 0);
    memset(kernel_dir, 0, sizeof(page_dir_t));
    currentd_dir = kernel_dir;

    for(int i = 1; i < 1024; i++)
    {
        kernel_dir->tables[i].user = 0;
        kernel_dir->tables[i].read_write = 1;
        kernel_dir->tables[i].present = 0;
    }

    for(int i = 0; i < 1024; i++)
    {
        first_page_table->pages[i].address = i * 0x1000 >> 12;
        first_page_table->pages[i].user = 0;
        first_page_table->pages[i].read_write = 1;
        first_page_table->pages[i].present = 1;
    }

    kernel_dir->tables[0].address = (uint32_t)first_page_table->pages >> 12;
    kernel_dir->tables[0].user = 0;
    kernel_dir->tables[0].read_write = 1;
    kernel_dir->tables[0].present = 1;

    load_page_dir(kernel_dir->tables);
    enable_paging();
}