#pragma once
#include <stdint.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

// the extended header 
#define EXT_HEADER_SIZE 48 // size of the extended header in bytes

typedef struct ext_header_1
{
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbus_cis_pointer;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base;
    uint8_t capabilities_ptr;
    uint8_t rsv0;
    uint16_t rsv1;
    uint32_t rsv2;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} ext_header_1_t;

typedef struct pci_device
{
    uint32_t bus;
    uint32_t device;
    uint32_t func;

    uint16_t vendor_id;
    uint16_t device_id;

    uint16_t cmd;
    uint16_t status;

    uint8_t revision_id;
    uint8_t prog_interface_id;
    uint8_t subclass_id;
    uint8_t class_id;
    
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;

    uint32_t ext_header[12];
} pci_device_t;

pci_device_t pci_devices[256];
int dev_index;

void pic_enumerate();