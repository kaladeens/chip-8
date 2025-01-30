#include <iostream>
#include <cstdint>
#include <fstream>
#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16
const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

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
public:
    chip8(char *game);

    void loadGame(char *game){
        std::ifstream file(game, std::ios::binary | std::ios::ate);
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
            std::cout << "ERROR: Unable to open file" << std::endl;
            exit(1);
        }
    }
        
    void initialize(char *game){
        std::fill(std::begin(memory), std::end(memory), 0);
        std::fill(std::begin(registers), std::end(registers), 0);
        std::fill(std::begin(stack), std::end(stack), 0);
        std::fill(std::begin(keys), std::end(keys), 0);
        std::fill(std::begin(display), std::end(display), 0);
        for (int i = 0; i < 80; i++){
            memory[FONTSET_START_ADDRESS + i] = fontset[i];
        }
        loadGame(game);
        pc = 0x200;
        I = 0;
        delayTimer = 0;
        soundTimer = 0;
        stackPointer = 0;
    }

    void main_loop(){
        while (true){
            uint16_t opcode = cycle();
            process_instruction(opcode);
        }
    }

    uint16_t cycle(){
        uint16_t opcode = memory[pc] || memory[pc+1];
        pc += 2;
        if(delayTimer > 0)
            --delayTimer;
        
        if(soundTimer > 0)
        {
            if(soundTimer == 1)
            printf("BEEP!\n");
            --soundTimer;
        }  
        return opcode;
    }
    // decode opcode and execute command here
    void process_instruction(uint16_t opcode){
        uint16_t msh = (opcode & 0xF000);
        uint16_t lsh = (opcode & 0x000F);

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
        else if (msh >= 0x3000 || msh < 0xA000){
            uint8_t x = (opcode & 0x0F00) >> 8; 

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
            else if (msh == 0x5000 || msh >= 0x8000){
                uint8_t y = (opcode & 0x00F0) >> 4;

                if (msh == 0x5000){
                    if (registers[x] == registers[y]){
                        pc += 2;
                    } 
                } 
                // Various arithmetic and logic operations
                else if (msh == 0x8000){
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
            registers[x] = (0 + rand() % 256) & (opcode * 0x00FF);
        } 
        // Draw a sprite at coordinate (Vx, Vy)
        else if (msh == 0xD000){
            registers[0xF] = 0;
            uint8_t x = (opcode & 0x0F00) >> 8; 
            uint8_t y = (opcode & 0x00F0) >> 4; 
            uint8_t height = opcode & 0x000F;

            uint8_t x_pos = registers[x] % 64;
            uint8_t y_pos = registers[y] % 32;
            
            for (int i = 0; i < height; i++){
                uint8_t sprite = memory[I + i];
                for (int j = 0; j < 8; j++){
                    uint8_t pixel = sprite & (0x80 >> j);
                    uint8_t pos = (x_pos + j) + (y_pos + i) * 64;
                    if (pixel){
                        if (display[pos] == 1){
                            registers[0xF] = 1;
                        }
                        display[pos] ^= 1;
                    }
                }
            }
        }  

        else if (msh == 0xE000){

        }
    }
    
    ~chip8(){
        std::cout << "CHIP 8 FINISHED" << std::endl;
        delete[] memory;
        delete[] registers;
        delete[] stack;
        delete[] keys;
        delete[] display;
        delete[] fontset;
    };
};

// constructor
chip8::chip8(char *game)
{
    std::cout << "CHIP 8 STARTED" << std::endl;
    initialize(game);
}

// destructor
chip8::~chip8()
{
    std::cout << "CHIP 8 FINISHED" << std::endl;
    delete[] memory;
    delete[] registers;
    delete[] stack;
    delete[] keys;
    delete[] display;
    delete[] fontset;
}
