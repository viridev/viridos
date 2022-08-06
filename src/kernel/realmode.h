#include <stdint.h>

typedef struct
{
    uint16_t DI, SI, BP, SP, BX, DX, CX, AX;
    uint16_t GS, FS, ES, DS, EFlags;
} registers16_t __attribute__((packed));

void _int_16(uint8_t interrupt, registers16_t* regs);

typedef struct
{
    uint32_t EDI, ESI, EBP, ESP;
	uint32_t EBX, EDX, ECX, EAX;
	uint16_t FLAGS;
	uint16_t ES, DS, FS, GS;
} registers32_t __attribute__((packed));

void _int_32(uint8_t interrupt, registers16_t* regs);