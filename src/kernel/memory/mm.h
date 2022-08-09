#pragma once
#include <stdint.h>
#include <stddef.h>

uint32_t installed_memory;
// use &kernel_end for location
extern uint32_t kernel_start;
extern uint32_t kernel_end;

void* memset(void* dest, int c, uint32_t n);   
void* memcpy(void* dest, const void* src, uint32_t n);
void* memmove(void* dest, const void* src, uint32_t n);

void mm_init();

void* kmalloc(size_t size, int align, uint32_t *phys);