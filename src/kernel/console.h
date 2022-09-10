#pragma once
#include <stdint.h>
#include <stddef.h>

void printf(const char *str, ...);

void clear();
void console_log(char *str, ...);
void console_warn(char *str, ...);
void console_error(char *str, ...);

void console_init();