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

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

enum VGA_COLOR
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15,
};