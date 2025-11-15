#pragma once

#include <SDL2/SDL.h>
#include <deque>
#include "Point.hpp"

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
    bool hit_itself();
    void grow();
    bool is_point_on_body(Point p, bool skip_tail);

private:
    void draw_body_part(SDL_Renderer *renderer, Point part);
};