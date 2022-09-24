#include "console.h"
#include <utils/conversion.h>
#include <video/vga_text.h>
#include <memory/mem.h>
#include <devices/drivers/ps2/ps2_kb.h>
#include <devices/chips/pit.h>
#include <stdarg.h>

uint16_t *buffer = (uint16_t *)0xB8000;

int console_width = 80;
int console_height = 25;

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
    for (int i = 0; i < console_width * (console_height - 1); i++)
        buffer[i] = buffer[i + console_width];
    for (int i = 0; i < console_width; i++)
        buffer[console_width * (console_height - 1) + i] = ((uint16_t)color_bg << 12 | color_fg << 8) | ' ';
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

void console_clear()
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
char *itoa(uint64_t value, char *str, int base);

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
            if (str[i] == 's')
                print(va_arg(list, const char *));
            if (str[i] == 'd')
                print(itoa(va_arg(list, int), buf, 10));
            if (str[i] == 'x')
                print(itoa(va_arg(list, int), buf, 16));
            if (str[i] == 'c')
                put_char(va_arg(list, int));
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

    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] == '\n')
            str[i] = '\0';

    va_list list;
    va_start(list, 256);

    for (size_t i = 0; str[i] != '\0'; i++)
    {
        switch (str[i])
        {
        case '%':
            i++;
            if (str[i] == 's')
                print(va_arg(list, const char *));
            if (str[i] == 'd')
                print(itoa(va_arg(list, int), buf, 10));
            if (str[i] == 'x')
                print(itoa(va_arg(list, int), buf, 16));
            memset(buf, 0, 64);
            break;
        default:
            put_char(str[i]);
            break;
        }
    }

    print("\n");

    va_end(list);
}

void console_warn(char *str, ...)
{
    color_fg = YELLOW;
    print("[");
    color_fg = LIGHT_GREY;
    print("WARN");
    color_fg = YELLOW;
    print("]");
    color_fg = WHITE;

    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] == '\n')
            str[i] = '\0';

    va_list list;
    va_start(list, 256);

    for (size_t i = 0; str[i] != '\0'; i++)
    {
        switch (str[i])
        {
        case '%':
            i++;
            if (str[i] == 's')
                print(va_arg(list, const char *));
            if (str[i] == 'd')
                print(itoa(va_arg(list, int), buf, 10));
            if (str[i] == 'x')
                print(itoa(va_arg(list, int), buf, 16));
            break;
        default:
            put_char(str[i]);
            break;
        }
    }
    print("\n");

    va_end(list);
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

    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] == '\n')
            str[i] = '\0';

    va_list list;
    va_start(list, 256);

    for (size_t i = 0; str[i] != '\0'; i++)
    {
        switch (str[i])
        {
        case '%':
            i++;
            if (str[i] == 's')
                print(va_arg(list, const char *));
            if (str[i] == 'd')
                print(itoa(va_arg(list, int), buf, 10));
            if (str[i] == 'x')
                print(itoa(va_arg(list, int), buf, 16));
            memset(buf, 0, 64);
            break;
        default:
            put_char(str[i]);
            break;
        }
    }
    print("\n");

    va_end(list);
}

// spaces shall be used as non valid chars
const char *keys = "  1234567890-=\b\tqwertyuiop[]\n asdfghjkl;'` \\zxcvbnm,./";
const char *keys_shift = "  !@#$%^&*()_+\b\tQWERTYUIOP{}\n ASDFGHJKL:\"~ |ZXCVBNM<>?";

char *dest;
int loc;
int reading_input = 0;
uint8_t previous_key; // previous key
int repeating = 0;
int last_time;

void console_read_line(char *d)
{
    loc = 0;
    dest = d;
    repeating = 0;
    reading_input = 1;
    previous_key = 0;
    last_time = pit_get_elapsed_time();

    while (reading_input);

    loc = 0;
    repeating = 0;
    previous_key = 0;
    last_time = 0;
}

void console_on_key_down()
{
    if (!reading_input) return;

    if (loc == 256)
        reading_input = 0;

    uint8_t key = ps2_kb_get_last_key();
    if (key > 0x80) return;

    if (key == previous_key)
        if (repeating)
        {
            if (pit_get_elapsed_time() < last_time + 50)
                return;
        }
        else
        {
            if (pit_get_elapsed_time() < last_time + 500)
                return;
            repeating = 1;
        }            
    else
        repeating = 0;
        
    previous_key = key;
    last_time = pit_get_elapsed_time();

    char c;

    if (key == 0x39) // space
    {
        put_char(' ');
        dest[loc++] = ' ';
    }
    else if (key >= 0x0 && key <= 0x35)
    {
        if (ps2_kb_is_shifting())
            c = keys_shift[key];
        else
            c = keys[key];

        if (c != ' ')
            if (c == '\b' && loc > 0)
            {
                dec_x();
                put_char(' ');
                dec_x();
                dest[--loc] = '\0';
            }
            else if (c == '\n')
            {
                dest[loc] = '\0';
                put_char('\n');
                reading_input = 0;
            }
            else
            {
                put_char(c);
                dest[loc++] = c;
            }
    }
}

void console_init()
{
    console_clear();
}