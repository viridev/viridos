#include "elf.h"

bool elf_check_header(elf_header_t *hdr)
{
    if (!hdr) return false;

    // check magic
    if (hdr->magic[0] != 0x7f) return false;
    if (hdr->magic[1] != 'E') return false;
    if (hdr->magic[2] != 'L') return false;
    if (hdr->magic[3] != 'F') return false;

    if (hdr->bit_length_type != 1) return false; // 32bit
    if (hdr->endianness_type != 1) return false; // little-endian

    if (hdr->instruction_set != 3) return false; // 386 (x86)
    if (hdr->header_version != 1) return false; // 1

    if (!(hdr->type == 1 || hdr->type == 2)) return false; // relocatable and executable
    return true;
}