#include <iostream>
#include <cstdint>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16

class chip8

{
private:
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
public:
    chip8(/* args */);

    void decode_opcode(){
        uint16_t opcode = memory[pc] | memory[pc+1];

        if (opcode == 0x00E0){
            std::fill(std::begin(display), std::end(display), 0);
        } else if (opcode == 0x00EE) {
            pc = stack[stackPointer];
            stackPointer -= 1;
        } else if ((opcode & 0xF000) == 0x1000){ // for 1 lowkey beautiful it ands to only have first part then goes on
            pc = 0x0FFF & opcode;
        } else if ((opcode & 0xF000) == 0x2000){
            stackPointer += 1;
            stack[stackPointer] = pc;
            pc = 0x0FFF & opcode;
        } else if ((opcode & 0xF000) >= 0x3000 || (opcode & 0xF000) < 0xA000){
            uint8_t x = (opcode & 0x0F00) >> 8; 

            if ((opcode & 0xF000) == 0x3000){
                if (registers[x] == (opcode & 0x00FF)){
                    pc += 4; 
                } else {
                    pc += 2; 
                }
            } else if ((opcode & 0xF000) == 0x4000){
                if (registers[x] == (opcode & 0x00FF)){
                    pc += 4; 
                } else {
                    pc += 2; 
                }
            } else if ((opcode & 0xF000) == 0x5000 || (opcode & 0xF000) == 0x8000){
                uint8_t y = (opcode & 0x00F0) >> 4;

                if ((opcode & 0xF000) == 0x5000){
                    if (registers[x] == registers[y]){
                        pc += 2;
                    } else {
                        pc += 4;
                    }
                }
            }

            } 
        }
            
    }
    
    ~chip8(){
        std::cout << "CHIP 8 FINISHED" << std::endl;
    };
};

chip8::chip8(/* args */)
{
}

chip8::~chip8()
{
}
