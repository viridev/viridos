#pragma once
#include <stdbool.h>

extern unsigned char vgat_fg;
extern unsigned char vgat_bg;
extern int vgat_x;
extern int vgat_y;
extern bool vgat_cursor_follow;

void vgat_putc(unsigned char c);
void vgat_print(const char *string);
void vgat_fill(unsigned char color);
void vgat_rect(int x, int y, int w, int h, unsigned char color);
void vgat_cursor_disable();
void vgat_cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void vgat_cursor_update(int x, int y);
uint16_t vgat_cursor_position();