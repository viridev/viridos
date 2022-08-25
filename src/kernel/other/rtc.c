#include "rtc.h"

static int bcdtobin(int value)
{
    return (value >> 4) * 10 + (value & 15);
}

int rtc_year()
{
    outb(0x70, REG_YEAR);
    return bcdtobin(inb(0x71));
}

int rtc_month()
{
    outb(0x70, REG_MONTH);
    return bcdtobin(inb(0x71));
}

int rtc_day()
{
    outb(0x70, REG_DAY);
    return bcdtobin(inb(0x71));
}

int rtc_day_of_the_weak()
{
    outb(0x70, REG_DAY_OF_THE_WEEK);
    return bcdtobin(inb(0x71));
}

int rtc_hour()
{
    outb(0x70, REG_HOUR);
    return bcdtobin(inb(0x71));
}

int rtc_minute()
{
    outb(0x70, REG_MIN);
    return bcdtobin(inb(0x71));
}

int rtc_second()
{
    outb(0x70, REG_SEC);
    return bcdtobin(inb(0x71));
}