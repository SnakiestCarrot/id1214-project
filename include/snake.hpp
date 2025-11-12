
#pragma once

#include <iostream>
#include <SDL2/SDL.h>

class Snake {
public:
    Snake();

    void update();
    void draw(SDL_Renderer *renderer);

private:
    int x, y;
};