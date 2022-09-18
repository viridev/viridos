#pragma once
#include <stdint.h>

#define REG_SEC                         0x00
#define REG_MIN                         0x02
#define REG_HOUR                        0x04
#define REG_DAY_OF_THE_WEEK             0x06
#define REG_DAY                         0x07
#define REG_MONTH                       0x08
#define REG_YEAR                        0x09

int rtc_year();
int rtc_month();
int rtc_day();
int rtc_day_of_the_weak();
int rtc_hour();
int rtc_minute();
int rtc_second();