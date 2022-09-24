#include "kshell.h"
#include <console.h>
#include <utils/str.h>

#include <devices/chips/rtc.h>
#include <devices/pci.h>

char cmd[256];
char label[9];

void parse_cmd()
{
    // convert everything to lower case
    for (int i = 0; i < 256; i++)
        if (cmd[i] >= 65 && cmd[i] <= 90)
            cmd[i] += 32;
    
    // get the label
    for (int i = 0; i < 8; i++)
        if (cmd[i] == '\0' || cmd[i] == ' ')
            break;
        else
            label[i] = cmd[i];    

    if (strcmp(label, "ping") == 0)
        printf("Pong!");
    else if (strcmp(label, "lspci") == 0)
    {
        for (int i = 0; i < dev_index; i++)
        {
            printf("%d.%d.%d (0x%x, 0x%x, 0x%x, 0x%x 0x%x)\n",
                pci_devices[i].bus, pci_devices[i].device, pci_devices[i].func,
                pci_devices[i].vendor_id, pci_devices[i].device_id, pci_devices[i].class_id, pci_devices[i].subclass_id, pci_devices[i].prog_interface_id);
        }
    }
    else if (strcmp(label, "version") == 0)
    {
        printf("Honestly I don't know either!");
    }
    else if (strcmp(label, "date") == 0)
    {
        printf("%d.%d.%d", rtc_year(), rtc_month(), rtc_day());
    }
    else if (strcmp(label, "time") == 0)
    {
        printf("%d.%d.%d", rtc_hour(), rtc_minute(), rtc_second());
    }


    if (cursor_x != 0)
        printf("\n");
}

void kshell_start()
{
    console_clear();
    while(1)
    {
        printf("> ");
        console_read_line(&cmd);
        parse_cmd();
    }    
}