#include "emulator.h"
#include "stdint.h"

#define RAM_BYTES 4096

//RAM
uint8_t memory[RAM_BYTES];

//General purpose registers
uint8_t rV0, rV1, rV2, rV3;
uint8_t rV4, rV5, rV6, rV7;
uint8_t rV8, rV9, rVA, rVB;
uint8_t rVC, rVD, rVE, rVF;

//'I' memory address register
uint16_t rI;

//Stack pointer and stack
uint8_t rSP;
uint16_t stack[16];

//Program Counter
uint16_t rPC;

//64x32 Monochrome display
uint8_t display[8][32]
