#include <cstdlib>
#include "chip8.hpp"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

constexpr uint32_t DISP_WIDTH = WIDTH * 10;
constexpr uint32_t DISP_HEIGHT = HEIGHT * 10;
SDL_Window *window;
SDL_Renderer *renderer;

std::map<SDL_Keycode, int> keymap = {
    {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
    {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
    {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
    {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}
};

bool init(){

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    
    atexit(SDL_Quit); //!chk this works 

    // screen = SDL_SetRenderViewport(renderer)
    if (SDL_CreateWindowAndRenderer("emulator",
        DISP_WIDTH,
        DISP_HEIGHT,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_KEYBOARD_GRABBED,
        &window, &renderer
    )){
        printf("screen created okayly\n");
    }
    else{
        printf("error fked this up when making window n renderer\n");
        return false;
    }

}



int main(int argc, char * argv[]){
    if (argc < 2){
        std::cout << "use it right dummy" << std::endl;
        return 1;
    }
    char *filename = argv[1];

    Chip8 chip8; 


    SDL_Event e;
    uint32_t key;
    bool running;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                else if (keymap.count(e.key.keysym.sym)) {
                    chip8.keys[keymap[e.key.keysym.sym]] = 1;
                }
            }
            else if (e.type == SDL_EVENT_KEY_UP) {
                if (keymap.count(e.key.keysym.sym)) {
                    keys[keymap[e.key.keysym.sym]] = 0;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

