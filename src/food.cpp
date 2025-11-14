
#include <SDL2/SDL.h>

#include "food.hpp"
#include "point.hpp"

const int CELL_SIZE = 20;

Food::Food(int x, int y) {
    this->position = Point{x, y};
}

void Food::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect;
    foodRect.x = position.x * CELL_SIZE;
    foodRect.y = position.y * CELL_SIZE;
    foodRect.w = CELL_SIZE;
    foodRect.h = CELL_SIZE;
    SDL_RenderFillRect(renderer, &foodRect);
}
