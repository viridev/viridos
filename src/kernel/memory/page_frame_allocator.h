#pragma once
#include <stdint.h>
#include <stddef.h>

void page_frame_allocator_init();

void free_page(void *address);
void free_pages(void *address, size_t count);

void lock_page(void *address);
void lock_pages(void *address, size_t count);

void reserve_page(void *address);
void reserve_pages(void *address, size_t count);

void* request_page();

size_t size_to_page_count(size_t size_bytes);

// get the amount of installed ram in bytes
size_t get_ram_size();
// get the amount of free ram in bytes
size_t get_free_ram();
// get the amount of used ram in bytes
size_t get_used_ram();

enum PAGE_STATUS
{
    PAGE_FREE,
    PAGE_LOCKED,
    PAGE_RESERVED,
};