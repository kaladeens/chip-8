#include "chip8.hpp"
#include <SFML/Graphics.hpp>

CHIP_8::chip8 chip8;
int modifier = 10;

// Window size
const unsigned int display_width = WIDTH * modifier;
const unsigned int display_height = HEIGHT * modifier;

sf::RenderWindow window(sf::VideoMode({display_width, display_height}), "CHIP 8 EMULATOR");

void updateKeys();
void init(char * filename);
void main_loop();
void updateGraphics();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }
    char *fileName = argv[1];
    // char *fileName = "br8kout.ch8";

    init(fileName);
    main_loop();

    return 0;
}

// initialise all of the stuff
void init(char *filename){
    chip8.initialize();
    chip8.loadGame(filename);
}

void main_loop(){
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                    updateKeys(); // Call updateKeys() when a key event is detected

                // for (int i = 0; i < 16; i++) {
                //     if (chip8.keys[i]) {
                //         std::cout << "Key " << i << ": Pressed" << std::endl;
                //     }
                // }

                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }  
        }
        // Debugging: Print the display buffer
        // for (int y = 0; y < 32; ++y) {
        //     for (int x = 0; x < 64; ++x) {
        //         std::cout << (chip8.display[(y * 64) + x] ? "X" : " ");
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << std::endl;
        chip8.cycle(); // Process one cycle of the CHIP-8

        if (chip8.drawFlag) { // Check if the draw flag is set
            window.clear();
            updateGraphics(); // Update the graphics
            window.display();
            chip8.drawFlag = false; // Reset the draw flag
        }
    }
}

void updateKeys() {
    chip8.keys[0x0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1);
    chip8.keys[0x1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2);
    chip8.keys[0x2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3);
    chip8.keys[0x3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4);
    chip8.keys[0x4] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q);
    chip8.keys[0x5] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W);
    chip8.keys[0x6] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E);
    chip8.keys[0x7] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::R);
    chip8.keys[0x8] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);
    chip8.keys[0x9] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S);
    chip8.keys[0xA] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);
    chip8.keys[0xB] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F);
    chip8.keys[0xC] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z);
    chip8.keys[0xD] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::X);
    chip8.keys[0xE] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C);
    chip8.keys[0xF] = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::V);
}

void updateGraphics(){
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            sf::RectangleShape pixel(sf::Vector2f(modifier, modifier));
            pixel.setPosition(sf::Vector2f(x * modifier, y * modifier));
            if (chip8.display[(y * WIDTH) + x] == 1) {
                pixel.setFillColor(sf::Color::White);
            } else {
                pixel.setFillColor(sf::Color::Black);
            }
            window.draw(pixel);
        }
    }
}
