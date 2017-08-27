#ifndef CHIP8_EMULATOR
#define CHIP8_EMULATOR

#define GETXREG() uint8_t* xReg = getRegister(getX());

#define GETYREG() uint8_t* yReg = getRegister(getY());

#define GETXYREG()    GETXREG\
                    GETYREG



#endif
