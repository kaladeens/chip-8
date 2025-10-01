#include <cstdlib.h>
#include <stack>

#define RAM_SIZE 4 * 1024 //memory size 
#define STACK_SIZE 16
#define HEIGHT 32
#define WIDTH 64
#define REG_SIZE 16

class Chip8 { 
  public:
    uint8_t mem[RAM_SIZE];
    bool screen[HEIGHT][WIDTH]; //64 pixels wide and 32 pixels high
    uint8_t dltim;
    uint8_t sndtim;
  private:
    // functions
    uint16_t fetch();
    
    //variables
    uint16_t pc;
    uint16_t ir; // index reg pint > to mem locations
    std::stack<uint16_t> kstk;
    uint8_t gpr[REG_SIZE];
}

