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
  
  uint8_t id = (0xF000 & inst) >> 12;
  uint8_t x = (0x0F00 & inst) >> 8;
  uint8_t y = (0x00F0 & inst) >> 8;
  uint16_t X = gpr[x];
  uint16_t Y = gpt[y];
  uint8_t N = 0x000F & inst;
  uint16_t NN = 0x00FF & inst;
  uint16_t NNN = 0x0FFF & inst;
  switch (id){
  case 0:
    // 00E0
    switch (N){
      case 0:     
      for (uint8_t i = 0; i < HEIGHT; i++){
        for (uint8_t j = 0; j < WIDTH; j++){
          screen[i][j] = false;
        }
      }
      // 00EE 
      case 0xE:
      pc = stk.pop()
  case 1:
    // 1NNN
    pc = NNN;
  case 2:
    // 2NNN
    stk.push(pc);
    pc = NNN;
  case 3:
    //3xkk
    if (X == NN){
      pc += 2;
    }
    break;
  case 4:
    //4xkk
    if (X != NN){
      pc += 2;
    }
    break;
  case 5:
    //5xy0
    if (X == Y){
      pc += 2;
    }
    break;
  case 6:
    //6xkk
    gpr[x] = NN;
    break;
  }
  case 7:
    //7xkk
    gpr[x] += NN;
    break;
  case 8:
    // 8xy?
    switch (N){
      case 0:
        gpr[x] = Y;
      case 1:
        gpr[x] = Y | X;
      case 2:
        gpr[x] = Y & X;
        break;
      case 3:
        gpr[x] = Y ^ X;
        break;
      case 4:
        uint16_t res = X + Y;
        if (res > 255){
          gpr[Vf] = 1;
        }
        gpr[x] = (uint8_t) res;
        break;
      case 5:
        gpr[Vf] = 0;
        if (X > Y){
          gpr[Vf] = 1;
        }
        gpr[x] = X - Y;
        break;
      case 6:
        uint8_t lsb = X & 0x01;
        gpr[Vf] = lsb;
        gpr[x] = X >> 1;
        break;
      case 7:
        gpr[Vf] = 0;
        if (Y > X){
          gpr[Vf] = 1;
        }
        gpr[x] = X - Y;
        break;
      case 0xE:
        uint8_t msb = (Y & 0x80) >> 7;
        gpr[Vf] = msb;
        gpr[Y] = X << 1;
        break;
    }
    break;
  case 9:
    if (X != Y){
      pc += 2;
    }
    break;
  case 0xA:
    ir = NNN;
    break;
  case 0xB:
    uint16_t jmpaddr = gpr[0] + NNN;
    pc = jmpaddr;
    break;
  case 0xC:
    //TODO random 
    uint8_t rand = std::rand() % 255; // rand 8 bit val
    uint8_t nnrand = rand & NN;
    gpr[x] = nnrand;
    break;
  case 0xD:
    uint8_t xpos = X; 
    uint8_t ypos = WIDTH * Y ; // * width to get to a row so if 
    // loop over n bytes the bytes are gonna be representative of 
    // the rows then move across the row length of 8
    for (uint8_t i = xpos; i < N; i++){
      uint8_t i_mod = i % HEIGHT;
      for (uint8_t j = ypos; j < 8; j++){
        uint8_t j_mod = j % WIDTH;
        bool scr = screen[i_mod][j_mod];
        bool bit = mem[ir + i*N + j];
        bool nscr = scr ^ bit;
        screen[i_mod][j_mod] = nscr;
        if (scr & bit){ // for collision
          gpr[Vf] = 1;
        }
      }
    }
    break;
    case 0xE: //TODO kepress instructions
      switch (N){
        case 1:
          
        break;
        case 0xE:

        break;
        
      }
    break;
    case 0xF:
      switch (N){
        case 3:
          uint8_t d1 = X % 100;
          X = X / 100;
          uint8_t d2 = X % 10;
          X = X / 10;
          uint8_t d3 = X % 1;
          mem[ir] = d1;
          mem[ir + 1] = d2;
          mem[ir + 2] = d3;
        break;
        case 7:
          gpr[x] = dltim;
        break;
        case 8:
          sndtim = X;
        break;
        case 9:
          ir = 5 * X; // cos size of 5 per font  
        break;
        case 0xE:
          ir += X;
        break;
        case 0x5:
          switch ()
      break;
      }
}
