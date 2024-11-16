#include "chip8_st.h"

void decode_opcode(Chip8 *chip){
    uint16_t opcode = chip->memory[chip->pc] | chip->memory[chip->pc + 1]; // combint this and next partof inst

    if (opcode == 0x00E0){
        clear_display(chip);
    } elif (opcode == 0x00EE){
        
    }
}

void clear_display(Chip8 *chip){
    for(int i = 0; i < 32; i++){
        for (int j = 0; j < 64; j++){
            chip->display[i * 64 + j] = 0;
        }
    }
}