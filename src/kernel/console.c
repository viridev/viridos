#include "console.h"
#include <video/vbe.h>
#include <video/vga_text.h>
#include <memory/mem.h>
#include <stdarg.h>

uint16_t *buffer = (uint16_t *)0xB8000;

int console_width = 80;
int console_height = 25;

int cursor_x = 0;
int cursor_y = 0;

uint8_t color_fg = 0xF;
uint8_t color_bg = 0x0;

static void inc_x()
{
    cursor_x++;
    if (cursor_x == console_width)
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
        cursor_x = console_height - 1;
        cursor_y--;
    }
}

static void scroll()
{
    cursor_y = console_height - 1;
    cursor_x = 0;
    for (int i = 0; i < console_width * (console_height-1); i++)
        buffer[i] = buffer[i + console_width];
    for (int i = 0; i < console_width; i++)
        buffer[console_width * (console_height-1) + i] = ((uint16_t)color_bg << 12 | color_fg << 8) | ' ';
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
        buffer[cursor_y * console_width + cursor_x] = color_bg << 12 | color_fg << 8 | c;
        inc_x();
        break;
    }
    if (cursor_y >= console_height)
        scroll();
}

void clear()
{
    for (size_t i = 0; i < console_width * console_height; i++)
        buffer[i] = color_bg << 12 | color_fg << 8 | ' ';
    cursor_x = 0;
    cursor_y = 0;
}

static void print(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        put_char(str[i]);
}

// prototype
char* itoa(uint64_t value, char * str, int base);

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
            if (str[i] == 'c') put_char(va_arg(list, int));
            break;
        default:
            put_char(str[i]);
            break;
        }
    }
        
    va_end(list);
}

void console_log(char *str, ...)
{
    color_fg = DARK_GREY;
    print("[");
    color_fg = LIGHT_GREY;
    print("LOG");
    color_fg = DARK_GREY;
    print("]");
    color_fg = WHITE;

    for(size_t i = 0; str[i] != '\0'; i++)
        if(str[i] == '\n')
            str[i] = '\0';

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
            memset(buf, 0, 64);
            break;
        default:
            put_char(str[i]);
            break;
        }
    }

    print("\n");
        
    va_end(list);

    console_update();
}

void console_warn(char *str, ...)
{
    color_fg = DARK_GREY;
    print("[");
    color_fg = LIGHT_GREY;
    print("WARN");
    color_fg = YELLOW;
    print("]");
    color_fg = WHITE;

    for(size_t i = 0; str[i] != '\0'; i++)
        if(str[i] == '\n')
            str[i] = '\0';

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
    print("\n");
        
    va_end(list);

    console_update();
}

void console_error(char *str, ...)
{
    color_fg = DARK_GREY;
    print("[");
    color_fg = RED;
    print("ERROR");
    color_fg = DARK_GREY;
    print("]");
    color_fg = WHITE;

    for(size_t i = 0; str[i] != '\0'; i++)
        if(str[i] == '\n')
            str[i] = '\0';

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
            memset(buf, 0, 64);
            break;
        default:
            put_char(str[i]);
            break;
        }
    }
    print("\n");
        
    va_end(list);

    console_update();
}

void console_init()
{
    clear();
}

void console_update()
{ 
    
}

char* itoa(uint64_t value, char * str, int base)
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