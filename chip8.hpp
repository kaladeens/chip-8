#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16
#define WIDTH 64
#define HEIGHT 32

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

namespace CHIP_8 {
    class chip8 {
        public: 
            chip8(); // Constructor
            ~chip8(); // Destructor
            void loadGame(char* filename);
            void initialize();
            bool drawFlag;
            void process_instruction(uint16_t opcode);
            void cycle();
            uint8_t keys[16]; // Keypad state
            uint8_t display[WIDTH * HEIGHT]; // Display (64x32 pixels)

        private:
            uint8_t memory[MEMORY_SIZE];
            uint8_t registers[REGISTER_COUNT];
            uint16_t I; // Index register
            uint16_t pc; // Program counter
            uint8_t delayTimer;
            uint8_t soundTimer;
            uint16_t stack[16];
            uint16_t stackPointer; // Stack pointer
            uint8_t fontset[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
    };
}

#endif // CHIP8_HPP