#include <stdint.h>
#include <stdlib.h>
#include "bcd.h"

uint8_t getHundreds(uint32_t scratch) {return (uint8_t)((scratch & BCD_HUNDREDS_BLOCK) >> 16);}

uint8_t getTens(uint32_t scratch) {return (uint8_t)((scratch & BCD_TENS_BLOCK) >> 12);}

uint8_t getOnes(uint32_t scratch) {return (uint8_t)((scratch & BCD_ONES_BLOCK) >> 8);}

int checkHundreds(uint32_t scratch) {return getHundreds(scratch) > 4 ? 1 : 0;}

int checkTens(uint32_t scratch) {return getTens(scratch) > 4 ? 1 : 0;}

int checkOnes(uint32_t scratch) {return getOnes(scratch) > 4 ? 1 : 0;}

void updateHundreds(uint32_t* scratch) {*scratch = *scratch + (3 << 16);}

void updateTens(uint32_t* scratch) {*scratch = *scratch + (3 << 12);}

void updateOnes(uint32_t* scratch) {*scratch = *scratch + (3 << 8);}

void checkBlocks(uint32_t* scratch) {
    if(checkHundreds(*scratch)) {
        updateHundreds(scratch);
    }
    if(checkTens(*scratch)) {
        updateTens(scratch);
    }
    if(checkOnes(*scratch)) {
        updateOnes(scratch);
    }
}

BCD* getBCD(uint8_t binary) {
    uint32_t scratch = (uint32_t)binary;
    BCD* bcdRep = malloc(sizeof(BCD));
    if(bcdRep == NULL) {
        return NULL;
    }

    int i;
    for(i = 0; i < BCD_ITERATIONS; i++) {
        checkBlocks(&scratch);
        scratch = scratch << 1;
    }

    bcdRep->hundreds = getHundreds(scratch);
    bcdRep->tens = getTens(scratch);
    bcdRep->ones = getOnes(scratch);

    return bcdRep;
}