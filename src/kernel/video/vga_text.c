#include "vga_text.h"
#include <memory/mem.h>
#include <stdarg.h>

uint16_t *vga_buffer = (uint16_t *)0xB8000;

const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;

int cursor_x = 0;
int cursor_y = 0;
uint8_t color_fg = 0xF;
uint8_t color_bg = 0x0;

// increment x
static void inc_x()
{
    cursor_x++;
    if (cursor_x == VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }
}
// decrement x
static void dec_x()
{
    cursor_x--;
    if (cursor_x == -1)
    {
        cursor_x = VGA_WIDTH - 1;
        cursor_y--;
    }
}

static void update_cursor_pos()
{
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void scroll()
{
    cursor_y = VGA_HEIGHT - 1;
    cursor_x = 0;
    memcpy(0xB8000, 0xB8000 + VGA_WIDTH * 2, VGA_WIDTH * VGA_HEIGHT * 2 - VGA_WIDTH * 2);
    for (int i = 0; i < VGA_WIDTH; i++)
        vga_buffer[VGA_WIDTH * (VGA_HEIGHT-1) + i] = ((uint16_t)color_bg << 12 | color_fg << 8) | ' ';
    update_cursor_pos();
}

static void put_char(char c)
{
    switch (c)
    {
    case '\n':
        cursor_x = 0;
        cursor_y++;
        break;
    default:
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = ((uint16_t)color_bg << 12 | color_fg << 8) | c;
        inc_x();
        break;
    }
    if (cursor_y >= VGA_HEIGHT)
        scroll();
    update_cursor_pos();
}

void clear()
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        put_char(' ');
    cursor_x = 0;
    cursor_y = 0;
    update_cursor_pos();
}

void print(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        put_char(str[i]);
}

// prototype
char* itoa(int value, char * str, int base);

static char buf[256];
void printf(const char *str, ...)
{
    va_list list;
    va_start(list, 256);

    for (size_t i = 0; str[i] != '\0'; i++)
    {
        switch (str[i])
        {
        case '%':
            i++;
            if (str[i] == 's') print(va_arg(list, const char*));
            if (str[i] == 'd') print(itoa(va_arg(list, int), buf, 10));
            if (str[i] == 'x') print(itoa(va_arg(list, int), buf, 16));
            break;
        default:
            put_char(str[i]);
            break;
        }
    }
        
    va_end(list);
}

void console_init()
{
    clear();
    print("VGA Console initialized.\n");
}

char* itoa(int value, char * str, int base)
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}