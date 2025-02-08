#include "chip8.hpp"
#include <chrono>
#include <thread>

namespace CHIP_8 {
        void chip8::loadGame(char *filename){
            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (file.is_open())  {
                std::streampos size = file.tellg();
                char* buffer = new char[size];
                file.seekg(0, std::ios::beg);
                file.read(buffer, size);
                file.close();

                for (int i = 0; i < size; i++){
                    memory[i + START_ADDRESS] = buffer[i];
                }

                delete[] buffer;
            }
            else {
                std::cerr << "Failed to open file: " << filename << std::endl;
                exit(1);
            }
        }

        void chip8::initialize(){
            std::fill(std::begin(memory), std::end(memory), 0);
            std::fill(std::begin(registers), std::end(registers), 0);
            std::fill(std::begin(stack), std::end(stack), 0);
            std::fill(std::begin(keys), std::end(keys), 0);
            std::fill(std::begin(display), std::end(display), 0);
            for (int i = 0; i < 80; i++){
                memory[FONTSET_START_ADDRESS + i] = fontset[i];
            }
            pc = 0x200;
            I = 0;
            delayTimer = 0;
            soundTimer = 0;
            stackPointer = 0;
        }

        void chip8::cycle(){
            uint16_t opcode = (memory[pc] << 8) | memory[pc+1];
            pc += 2;
            if(delayTimer > 0)
                --delayTimer;
            if(soundTimer > 0)
            {
                if(soundTimer == 1){
                    std::cout << "BEEP!\n" << std::endl;
                }
                --soundTimer;

            }  
            process_instruction(opcode);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // decode opcode and execute command here
        void chip8::process_instruction(uint16_t opcode){
            uint16_t msh = (opcode & 0xF000);
            uint16_t lsh = (opcode & 0x000F);
            // std::cout << std::hex << opcode << std::endl;
            // Clear the display
            if (opcode == 0x00E0){
                std::fill(std::begin(display), std::end(display), 0);
            } 
            // Return from subroutine
            else if (opcode == 0x00EE) {
                stackPointer -= 1;
                pc = stack[stackPointer];
            } 
            // Jump to address NNN
            else if (msh == 0x1000){
                pc = 0x0FFF & opcode;
            } 
            // Call subroutine at NNN
            else if (msh == 0x2000){
                stack[stackPointer] = pc;
                stackPointer += 1;
                pc = 0x0FFF & opcode;
            } 
            // Skip next instruction if Vx == NN
            else if (msh >= 0x3000 && msh < 0xA000){
                uint8_t x = (opcode & 0x0F00) >> 8;
                x &= 0x0F; // Ensure x is within the range 0-15

                if (msh == 0x3000){
                    if (registers[x] == (opcode & 0x00FF)){
                        pc += 2; 
                    } 
                } 
                // Skip next instruction if Vx != NN
                else if (msh == 0x4000){
                    if (registers[x] != (opcode & 0x00FF)){
                        pc += 2; 
                    } 
                } 
                // Skip next instruction if Vx == Vy
                else {
                    uint8_t y = (opcode & 0x00F0) >> 4;

                    if (msh == 0x5000){
                        if (registers[x] == registers[y]){
                            pc += 2;
                        } 
                    } 
                    else if (msh == 0x6000){
                        registers[x] = opcode & 0x00FF;
                    } 
                    else if (msh == 0x7000){
                        registers[x] += opcode & 0x00FF;
                    }
                    // Various arithmetic and logic operations
                    else if (msh == 0x8000){
                        if (lsh == 0x0000){
                            registers[x] = registers[y];
                        } else if (lsh == 0x0001){
                            registers[x] = registers[x] | registers[y];
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
                    } 
                    // Skip next instruction if Vx != Vy
                    else if (msh == 0x9000){
                        if (lsh == 0x0000){
                            if (registers[x] != registers[y]){
                                pc += 2;
                            } 
                        }
                    }
                }
            } 
            // Set I to the address NNN
            else if (msh == 0xA000){
                I = opcode & 0x0FFF;
            } 
            // Jump to address NNN + V0
            else if (msh == 0xB000){
                pc = (opcode & 0x0FFF) + registers[0];
            } 
            // Set Vx to a random number and NN
            else if (msh == 0xC000){
                uint8_t x = (opcode & 0x0F00) >> 8; 
                registers[x] = (rand() % 256) & (opcode & 0x00FF);
            } 
            // Draw a sprite at coordinate (Vx, Vy)
            else if (msh == 0xD000) {
                drawFlag = true;
                registers[0xF] = 0; // Reset collision flag

                uint8_t x = (opcode & 0x0F00) >> 8; 
                uint8_t y = (opcode & 0x00F0) >> 4; 
                uint8_t height = opcode & 0x000F;

                uint8_t x_pos = registers[x] % 64;
                uint8_t y_pos = registers[y] % 32;

                for (int i = 0; i < height; i++) {
                    uint8_t sprite = memory[I + i];

                    for (int j = 0; j < 8; j++) {
                        if (x_pos + j >= 64) continue;  // Prevent drawing out of bounds (X-axis)
                        if (y_pos + i >= 32) continue;  // Prevent drawing out of bounds (Y-axis)

                        uint8_t pixel = (sprite >> (7 - j)) & 1; // Extract pixel as 0 or 1
                        uint16_t pos = (x_pos + j) + ((y_pos + i) * 64); 

                        if (pixel) {  
                            if (display[pos] == 1) {
                                registers[0xF] = 1; // Set collision flag if a pixel is erased
                            }
                            display[pos] ^= 1; // XOR pixel (toggle on/off)
                        }
                    }
                }
            }
            else if (msh == 0xE000 || msh == 0xF000){
                uint8_t x = (opcode & 0x0F00) >> 8;

                if (msh == 0xE000){
                    if (lsh == 0x0001){
                        if (!keys[registers[x]]){
                            pc +=2;
                        }
                    } 
                    else if (lsh == 0x000E){
                        if (keys[registers[x]]){
                            pc +=2;
                        }
                    }
                }
                else if (msh == 0xF000){
                    if (lsh == 0x0003){
                        uint8_t value = registers[x];
                        // Ones-place
                        memory[I + 2] = value % 10;
                        value /= 10;

                        // Tens-place
                        memory[I + 1] = value % 10;
                        value /= 10;

                        // Hundreds-place
                        memory[I] = value % 10;
                    } 
                    else if ((opcode & 0x00FF) == 0x0015){
                        delayTimer = registers[x];
                    }
                    else if ((opcode & 0x00FF) == 0x0055){
                        for (size_t i =0; i < x;i++){
                            memory[I+i] = registers[i];
                        }
                    }
                    else if ((opcode & 0x00FF) == 0x0065){
                        for (size_t i =0; i < x;i++){
                            registers[i] = memory[I+i];
                        }                
                    }
                    else if (lsh == 0x0007){
                        registers[x] = delayTimer;
                    }
                    else if (lsh == 0x0008){
                        soundTimer = registers[x];
                    }
                    else if (lsh == 0x0009){
                        I = fontset[registers[x]*5];
                    }
                    else if (lsh == 0x000A){
                        size_t k;
                        for (k = 0; k <sizeof(keys); k++){
                            if (keys[k]){
                                registers[x] = k;
                            }
                        }
                        // stick to instruction
                        if (k==15){
                            pc -= 2;
                        }
                    }
                    else if (lsh == 0x000E){
                        I += registers[x];
                    }
                }
            } 
            else {
                std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            }
        }
   
    chip8::chip8(){
        std::cout << "CHIP 8 STARTED" << std::endl;
    }
    // destructor
    chip8::~chip8()
    {
        std::cout << "CHIP 8 FINISHED" << std::endl;
    }
}