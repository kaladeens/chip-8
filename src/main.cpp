#include <cstdlib>
#include <map>
#include <iostream>
#include "chip8.hpp"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_keycode.h"

constexpr uint8_t PX_SIZE = 10;
constexpr uint32_t DISP_WIDTH = WIDTH * PX_SIZE;
constexpr uint32_t DISP_HEIGHT = HEIGHT * PX_SIZE;

SDL_Window *window;
SDL_Renderer *renderer;

std::map<SDL_Keycode, int> keymap = {
    {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
    {SDLK_Q, 0x4}, {SDLK_W, 0x5}, {SDLK_E, 0x6}, {SDLK_R, 0xD},
    {SDLK_A, 0x7}, {SDLK_S, 0x8}, {SDLK_D, 0x9}, {SDLK_F, 0xE},
    {SDLK_Z, 0xA}, {SDLK_X, 0x0}, {SDLK_C, 0xB}, {SDLK_V, 0xF}
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
        std::cout << "screen created okayly" << std::endl;
    }
    else{
        std::cout << "error fked this up when making window n renderer" << std::endl;
        return false;
    }
    return true;
}



int main(int argc, char * argv[]){
    if (argc < 2){
        std::cout << "use it right dummy" << std::endl;
        return 1;
    }
    char *filename = argv[1];

    Chip8 chip8; 

    chip8.readfile(filename);
    init();    
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE) {
                    running = false;
                }
                else if (keymap.count(e.key.key)) {
                    chip8.keys[keymap[e.key.key]] = 1;
                }
            }
            else if (e.type == SDL_EVENT_KEY_UP) {
                if (keymap.count(e.key.key)) {
                    chip8.keys[keymap[e.key.key]] = 0;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw Chip8 screen buffer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white pixels
        for (uint32_t y = 0; y < DISP_HEIGHT; y++) {
            for (uint32_t x = 0; x < DISP_WIDTH; x++) {
                if (chip8.screen[y][x]) { // pixel ON
                  SDL_FRect rect = {
                    static_cast<float>(x*PX_SIZE),
                    static_cast<float>(y*PX_SIZE),
                    static_cast<float>(PX_SIZE),
                    static_cast<float>(PX_SIZE),
                  };
                  SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        // Present to window
        SDL_RenderPresent(renderer);

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

