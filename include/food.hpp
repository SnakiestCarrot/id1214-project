
#pragma once

#include <SDL2/SDL.h>

#include "point.hpp"

class Food {
public:
    Food(int x, int y);
    void draw(SDL_Renderer *renderer);
    Point position;
    void move_randomly(int max_x, int max_y);
};
