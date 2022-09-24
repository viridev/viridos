#pragma once
#include <stdint.h>

int strcmp(const char *x, const char *y)
{
    while (*x)
    {
        // if characters differ, or end of the second string is reached
        if (*x != *y) {
            break;
        }
 
        // move to the next pair of characters
        x++;
        y++;
    }
 
    // return the ASCII difference after converting `char*` to `unsigned char*`
    return *(const unsigned char*)x - *(const unsigned char*)y;
}