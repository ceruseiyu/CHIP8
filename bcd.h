#include <stdint.h>

#ifndef CHIP8_BCD
#define CHIP8_BCD

#define BCD_ITERATIONS 8
#define BCD_HUNDREDS_BLOCK  0x000F0000
#define BCD_TENS_BLOCK      0x0000F000
#define BCD_ONES_BLOCK      0x00000F00

typedef struct bcdrep {
    uint8_t hundreds;
    uint8_t tens;
    uint8_t ones;
} BCD;

BCD* getBCD(uint8_t binary);

#endif