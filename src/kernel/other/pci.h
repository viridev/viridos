#pragma once
#include <stdint.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

typedef struct pci_device
{
    uint32_t bus;
    uint32_t device;

    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t prog_interface_id;
} pci_device_t;


void pic_enumerate();