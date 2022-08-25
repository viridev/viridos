#pragma once
#include <stdint.h>

typedef struct
{
    unsigned int present : 1;
    unsigned int read_write : 1;
    unsigned int user : 1;
    unsigned int ignore : 9;
    uint32_t address : 20;
} page_dir_entry_t;

typedef struct
{
    unsigned int present : 1;
    unsigned int read_write : 1;
    unsigned int user : 1;
    unsigned int ignore : 9;
    uint32_t address : 20;
} page_table_entry_t;

typedef struct
{
    page_table_entry_t pages[1024];
} page_table_t;

typedef struct
{
    page_dir_entry_t tables[1024];
} page_dir_t;

void paging_init();