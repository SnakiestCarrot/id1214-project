#pragma once

#include <SDL2/SDL.h>
#include <deque>
#include "point.hpp"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
public:
    Snake();
    void update();
    void draw(SDL_Renderer *renderer);
    std::deque<Point> body;
    Direction direction;

private:
    void draw_body_part(SDL_Renderer *renderer, Point part);
};