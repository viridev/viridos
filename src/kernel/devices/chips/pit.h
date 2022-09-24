#pragma once
#include <stdint.h>

#define BASE_FREQ 1193182

void pit_init(int freq);
uint64_t pit_get_elapsed_time();