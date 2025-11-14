#include "snake.hpp"
#include <SDL2/SDL.h>
#include <iostream>

// We need to know the cell size to draw correctly.
// It's a const in main.cpp, so we'll redefine it here.
// A better design would be to pass this value in.
const int CELL_SIZE = 20;

Snake::Snake() {
    // Initialize the snake in the middle of the 50x50 grid
    // Head at (25, 25)
    body.push_front({25, 25});
    // Body parts following
    body.push_back({24, 25});
    body.push_back({23, 25});
    
    // Start by moving to the right
    direction = RIGHT;
}

void Snake::update() {
    // 1. Get the current head
    Point currentHead = body.front();
    Point newHead = currentHead;

    // 2. Calculate the new head's position based on direction
    switch (direction) {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
    }

    // 3. Add the new head to the front of the body
    body.push_front(newHead);

    // 4. Remove the tail (for now, we assume we didn't eat food)
    // TODO: Add a check for eating food. If food is eaten,
    // you would *skip* this pop_back() call.
    body.pop_back();
}

void Snake::draw(SDL_Renderer *renderer) {
    if (body.empty()) {
        return; // Nothing to draw
    }

    // 1. Draw the head (green)
    Point head = body.front();
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    draw_body_part(renderer, head);

    // 2. Draw the rest of the body (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    
    // Iterate from the second element to the end
    for (auto it = body.begin() + 1; it != body.end(); ++it) {
        draw_body_part(renderer, *it);
    }
}

void Snake::draw_body_part(SDL_Renderer *renderer, Point part) {
    // Convert grid coordinates (part.x, part.y) to pixel coordinates
    SDL_Rect rect = {
        part.x * CELL_SIZE, 
        part.y * CELL_SIZE, 
        CELL_SIZE, 
        CELL_SIZE
    };
    SDL_RenderFillRect(renderer, &rect);
}