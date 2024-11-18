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
        uint16_t opcode = memory[pc] || memory[pc+1];
        uint16_t msh = (opcode & 0xF000);
        uint16_t lsh = (opcode & 0x000F);

        if (opcode == 0x00E0){
            std::fill(std::begin(display), std::end(display), 0);
        } else if (opcode == 0x00EE) {
            pc = stack[stackPointer];
            stackPointer -= 1;
        } else if (msh == 0x1000){ // for 1 lowkey beautiful it ands to only have first part then goes on
            pc = 0x0FFF & opcode;
        } else if (msh == 0x2000){
            stackPointer += 1;
            stack[stackPointer] = pc;
            pc = 0x0FFF & opcode;
        } else if (msh >= 0x3000 || msh < 0xA000){
            uint8_t x = (opcode & 0x0F00) >> 8; 

            if (msh == 0x3000){
                if (registers[x] == (opcode & 0x00FF)){
                    pc += 4; 
                } else {
                    pc += 2; 
                }
            } else if (msh == 0x4000){
                if (registers[x] != (opcode & 0x00FF)){
                    pc += 4; 
                } else {
                    pc += 2; 
                }
            } else if (msh == 0x5000 || msh >= 0x8000){
                uint8_t y = (opcode & 0x00F0) >> 4;

                if (msh == 0x5000){
                    if (registers[x] == registers[y]){
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                } else if (msh == 0x8000){
                    if (lsh == 0x0000){
                        registers[x] = registers[y];
                    } else if (lsh == 0x0001){
                        registers[x] = registers[x] || registers[y];
                    } else if (lsh == 0x0002){
                        registers[x] = registers[x] & registers[y];
                    } else if (lsh == 0x0003){
                        registers[x] = registers[x] ^ registers[y];
                    } else if (lsh == 0x0004){
                        uint16_t sum = registers[x] + registers[y];
                        registers[0xF] = (sum > 0xFF) ? 1 : 0; 
                        registers[x] = sum & 0xFF;
                    } else if (lsh == 0x0005){
                        uint16_t sum = registers[x] - registers[y];
                        registers[0xF] = (registers[x] > registers[y]) ? 1 : 0;
                        registers[x] = sum;
                    } else if (lsh == 0x0006){
                        registers[0xF] = registers[x] & 0x01;
                        registers[x] = registers[x] >> 1;
                    } else if (lsh == 0x0007){
                        uint16_t sum = registers[x] - registers[y];
                        registers[0xF] = (registers[x] > registers[y]) ? 1 : 0;
                        registers[x] = sum;
                    } else if (lsh == 0x000E){
                        registers[0xF] = registers[x] >> 7;
                        registers[x] = registers[x] << 1;
                    }
                } else if (msh == 0x9000){
                    if (lsh == 0x0000){
                        if (registers[x] != registers[y]){
                            pc += 4;
                        } else {
                            pc += 2;
                        }
                    }
                }
            }
        } else if (msh == 0xA000){
            I = opcode & 0x0FFF;
        } else if (msh == 0xB000){
            pc = (opcode & 0x0FFF) + registers[0];
        } else if (msh == 0xC000){
            uint8_t x = (opcode & 0x0F00) >> 8; 
            registers[x] = (0 + rand() % 256) & (opcode * 0x00FF);
        } else if (msh == 0xD000){
            registers[0xF] = 0;
            uint8_t x = (opcode & 0x0F00) >> 8; 
            uint8_t y = (opcode & 0x00F0) >> 4; 
            uint8_t curr;

            for (int i = 0; i <(lsh); i++ ){
                curr = memory[I + i];
                display[(registers[x] % 64) * 32];
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
