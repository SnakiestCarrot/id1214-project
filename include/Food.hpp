
#pragma once

#include <SDL2/SDL.h>

#include "Point.hpp"

class Food {
public:
    /**
     * @brief Creates a new food at x and y.
     * @param x X coordinate of new food
     * @param y Y coordinate of new food
     */
    Food(int x, int y);

    /**
     * @brief Draws the food object
     * @param renderer The SDL renderer to use when drawing the food
     */
    void draw(SDL_Renderer *renderer);

    /**
     * Represents the position with a Point class
     */
    Point position;

    /**
     * @brief Sets the position to a random numbers between 0 and the max bounds.
     * @param max_x Max bound in X direction
     * @param max_y Max bound in Y direction
     */
    void move_randomly(int max_x, int max_y);
};
