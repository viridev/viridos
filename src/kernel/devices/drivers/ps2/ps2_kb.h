#pragma once
#include <stdint.h>

char kb_code_to_ascii(uint8_t key);
uint8_t ps2_kb_get_last_key();
int ps2_kb_is_shifting();
void ps2_kb_init();