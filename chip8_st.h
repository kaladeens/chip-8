#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16

    
typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t registers[REGISTER_COUNT];
    uint16_t pc;
    uint16_t I;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t stackPointer;
    uint16_t stack[16];
    uint8_t keys[16];
    uint8_t display[64 * 32];
} Chip8; 

#endif