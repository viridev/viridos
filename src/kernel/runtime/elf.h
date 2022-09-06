#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t magic[4];
    uint8_t bit_length_type;
    uint8_t endianness_type;
    uint8_t header_version;
    uint8_t os_abi;
    uint8_t padding[8];
    uint16_t type;
    uint8_t instruction_set;
    uint32_t version;
    uintptr_t program_entry;
    uintptr_t program_header;
    uintptr_t section_header;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_header_entry_size;
    uint16_t program_header_entries_num;
    uint16_t section_header_entry_size;
    uint16_t section_header_entries_num;
    uint16_t section_header_name_index;
} elf_header_t __attribute__((packed));

typedef struct
{
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addr_align;
    uint32_t entries_size;
} elf_section_header_t __attribute__((packed));;