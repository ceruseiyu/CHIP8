#include "emulator.h"
#include "stdint.h"
#include "stdlib.h"

//Size of RAM
#define RAM_BYTES 4096

//Start of program in RAM for normal and ETI programs
#define PROGRAM_START 0x200
#define PROGRAM_START_ETI 0x600

//Size of stack
#define STACK_SIZE 16

//DIsplay size, width is in bytes (8 * 8 = 64 pixels)
#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 8

//Bitflag identifiers
#define PIX_1 0x1
#define PIX_2 0x2
#define PIX_3 0x4
#define PIX_4 0x8
#define PIX_5 0x10
#define PIX_6 0x20
#define PIX_7 0x40
#define PIX_8 0x80

//Instruction sections
#define INS_ADDR 0x0FFF
#define INS_BYTE 0x00FF
#define INS_NIBBLE 0x000F
#define INS_X 0x0F00
#define INS_Y 0x00F0
#define INS_OPCODE 0xF000

//Function counts
#define BASE_FUNCTION_COUNT 2
#define ADDR_FUNCTION_COUNT 5

//Instruction IDs
#define INS_CLEAR_DISPLAY 0x00E0
#define INS_RETURN_SUB 0x00EE
#define INS_JUMP_ADDR 0x1000
#define INS_CALL_ADDR 0x2000
#define INS_SKIP_EQUAL 0x3000
#define INS_SKIP_NOT_EQUAL 0x4000
#define INS_SKIP_EQUAL_REG 0x5000
#define INS_LOAD_REGISTER 0x6000
#define INS_ADD 0x7000
#define INS_COPY_REGISTER 0x8000
#define INS_OR_REGISTERS 0x8001
#define INS_AND_REGISTERS 0x8002
#define INS_XOR_REGISTERS 0x8003
#define INS_ADD_REGISTERS 0x8004
#define INS_SUB_REGISTERS 0x8005
#define INS_SHIFT_RIGHT 0x8006
#define INS_SUBN_REGISTERS 0x8007
#define INS_SHIFT_LEFT 0x800E
#define INS_SKIP_NOT_EQUAL_REGISTERS 0x9000
#define INS_LOAD_I 0xA000
#define INS_JUMP_V0 0xB000
#define INS_RANDOM_AND 0xC000
#define INS_DRAW_SPRITE 0xD000
#define INS_SKIP_NOT_PRESSED 0xE0A1
#define INS_COPY_DT 0xF007
#define INS_COPY_KEY 0xF00A
#define INS_LOAD_DT 0xF015
#define INS_LOAD_ST 0xF018
#define INS_ADD_I 0xF01E
#define INS_LOAD_I_SPRITE 0xF029
#define INS_STORE_BCD 0xF033
#define INS_STORE_REGISTER 0xF055
#define INS_LOAD_REGISTER_MEMORY 0xF065


//RAM
uint8_t memory[RAM_BYTES];

//General purpose registers
uint8_t rV0, rV1, rV2, rV3;
uint8_t rV4, rV5, rV6, rV7;
uint8_t rV8, rV9, rVA, rVB;
uint8_t rVC, rVD, rVE, rVF;

uint8_t* generalRegisters = {&rV0, &rV1, &rV2, &rV3, &rV4, &rV5, &rV6, &rV7, &rV8, &rV9, &rVA, &rVB, &rVC, &rVD, &rVE, &rVF}

//'I' memory address register
uint16_t rI;

//Stack pointer and stack
uint8_t rSP;
uint16_t stack[STACK_SIZE];

//Program Counter
uint16_t rPC;

//Current Instruction
uint16_t curInstruction;

//64x32 Monochrome display
uint8_t display[8][32]

//Base functions without parameters
void (*baseFunctions[])() = {clearDisplay, returnSub}
uint16_t baseCalls[] = {INS_CLEAR_DISPLAY, INS_RETURN_SUB} 

//Addr functions
void (*addrFunctions[])() = {jump, call, loadI, jumpV0}
uint16_t addrCalls[] = {INS_JUMP_ADDR, INS_CALL_ADDR, INS_LOAD_I, INS_JUMP_V0}

//XKK functions
void (*xkkFunctions[])() = {skipEqual, skipNotEqual, loadRegister, add, randomAND}
uint16_t xkkCalls[] = {INS_SKIP_EQUAL, INS_SKIP_NOT_EQUAL, INS_LOAD_REGISTER, INS_ADD, INS_RANDOM_AND}

//Main function caller
void runInstruction() {
    if(checkBaseInstructions() != -1)
        return;
    if(checkAddrFunctions() != -1)
        return;
    if(checkXKKFunctions() != -1)
        return;

}

//Instruction checkers
int checkBaseInstructions() {
    int i;
    for(i = 0; i < BASE_FUNCTION_COUNT; i++) {
        if(baseCalls[i] == curInstruction) {
            (*baseFunctions[i])();
            return 1;
        }
    }
    return -1;
}

int checkAddrFunctions() {
    int i;
    uint16_t insOpCode = getOpCode();
    for(i = 0; i < ADDR_FUNCTION_COUNT; i++) {
        if(addrCalls[i] == insOpCode) {
            (*addrFunctions[i])();
            return 1;
        }
    }
    return -1;
}

int checkXKKFunctions() {
    int i;
    uint16_t insOpCode = getOpCode();
    for(i = 0; i < ADDR_FUNCTION_COUNT; i++) {
        if(xkkCalls[i] == insOpCode) {
            (*xkkFunctions[i])();
            return 1;
        }
    }
    return -1;
}

//Utility functions
uint16_t getAddr() {
    return curInstruction & INS_ADDR;
}

uint16_t getByte() {
    return curInstruction & INS_BYTE;
}

uint16_t getNibble() {
    return curInstruction & INS_NIBBLE;
}

uint16_t getX() {
    return (curInstruction & INS_X) >> 8;
}

uint16_t getY() {
    return (curInstruction & INS_Y) >> 4;
}

uint8_t* getRegister(uint16_t regNumber) {
    return generalRegisters(regNumber);
}

uint16_t getOpCode() {
    return curInstruction & INS_OPCODE;
}

//Base Instructions
void clearDisplay() {
    int i;
    int j;
    for(i = 0; i < DISPLAY_WIDTH; i++) {
        for(j = 0; j < DISPLAY_HEIGHT; j++) {
            display[i][j] = 0;
        }
    }
}

void returnSub() {
    rPC = stack[rSP];
    if(rSP > 0) {
        rSP--;
    }
}

//Addr Instructions

void jump() {
    rPC = getAddr();
}

void call() {
    if(rSP < STACK_SIZE) {
        rSP++;
        stack[rSP] = rSP;
        rSP = getAddr();
    }
}

void loadI() {
    rI = getAddr();
}

void jumpV0() {
    rPC = getAddr() + rV0;
}

//XKK instructions

void skipEqual() {
    uint8_t* reg = getRegister(getX());
    uint16_t byte = getByte();
    if(*reg == (uint8_t)byte) {
        rPC++;
    }
}

void skipNotEqual() {
    uint8_t* reg = getRegister(getX());
    uint16_t byte = getByte();
    if(*reg != (uint8_t)byte) {
        rPC++;
    }
}

void loadRegister() {
    uint8_t* reg = getRegister(getX());
    uint16_t byte = getByte();
    *reg = (uint8_t)byte;
}

void add() {
    uint8_t* reg = getRegister(getX());
    uint16_t byte = getByte();
    *reg = *reg + (uint8_t)byte;
}

void randomAND() {
    uint8_t* reg = getRegister(getX());
    uint16_t byte = getByte();
    uint8_t ran = rand() % 255;
    *reg = ran & (uint8_t)byte;
}   