#pragma once

#include "snake.hpp"
#include "food.hpp"
#include <SDL2/SDL.h>

class World {
public:
    World(Snake& snake, Food& food);
    void update();
    void draw(SDL_Renderer *renderer);
    void handle_input(SDL_Event& event);
    void reset();

private:
    Snake& snake;
    Food& food;
    int width;
    int height;
    int cell_size;
    int score;
    void draw_grid(SDL_Renderer *renderer);
    bool snake_is_eating_food();
    bool snake_hit_wall();
};