#include "main.hpp"

// ad should always be pc but just in case leave like this //TODO
uint16_t Chip8::fetch(){
  uint16_t mskd_pc = 0x0FFF & pc;
  uint16_t instruction = pc[mskd_pc] << 8 | pc[mskd_pc + 1];
  pc += 2;
  return instruction;
}

uint16_t Chip8::decycle(){
  uint16_t inst = fetch();
  
  uint8_t id = 0xF000 & inst;
  uint8_t x = 0x0F00 & inst;
  uint8_t y = 0x00F0 & inst;
  uint8_t N = 0x000F & inst;
  uint16_t NN = 0x0FFF & inst;
  int16_t NNN = 0x0FFF & inst;
  switch (id){
    case 0:
      for (uint8_t i; i < HEIGHT; i++){
        for uint8_t j; i < WIDTH; j++){
          screen[i][j] = false;
        }
      }
    case 1:
  }
}
