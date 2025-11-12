#include "snake.hpp"
#include <SDL2/SDL.h>

// This is the "implementation" file.
// It #includes the header and provides the code
// for the functions declared in snake.hpp.

Snake::Snake() {
    // We can initialize variables here
    std::cout << "A new Snake has been created!" << std::endl;
    x = 100;
    y = 100;
}

void Snake::update() {
    std::cout << "Snake is updating..." << std::endl;
}

void Snake::draw(SDL_Renderer *renderer) {
    std::cout << "Snake is drawing..." << std::endl;
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, this->x, this->y);
}