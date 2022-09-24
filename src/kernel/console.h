#pragma once
#include <stdint.h>
#include <stddef.h>

int cursor_x;
int cursor_y;

void printf(const char *str, ...);

void console_clear();
void console_log(char *str, ...);
void console_warn(char *str, ...);
void console_error(char *str, ...);

void console_wait_for_key();
void console_read_line(char *dest);
void console_on_key_down();

void console_init();