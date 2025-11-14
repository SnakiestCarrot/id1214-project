
#include <iostream>

#include "world.hpp"
#include "snake.hpp"
#include "food.hpp"

// Use an initializer list to store the reference
World::World(Snake& snake, Food& food) : snake(snake), food(food) {
    this->width = 1000;
    this->height = 600;
    this->cell_size = 20;
    this->score = 0;
}

void World::draw(SDL_Renderer *renderer) {
    this->draw_grid(renderer);
    this->food.draw(renderer);
    this->snake.draw(renderer);
}

void World::draw_grid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    
    for (int i = 0; i <= height; i += cell_size) {
        SDL_RenderDrawLine(renderer, 0, i, width, i);
    }
    for (int j = 0; j <= width; j += cell_size) {
        SDL_RenderDrawLine(renderer, j, 0, j, height);
    }
}

void World::update() {
    snake.update();

    if(snake_hit_wall() || snake.hit_itself()) {
        reset();
    }

    if(snake_is_eating_food()) {
        snake.grow();

        // check food is not spawned on the snake body
        bool onSnake;
        int max_x = width / cell_size;
        int max_y = height / cell_size;
        do {
            onSnake = false;

            food.move_randomly(max_x, max_y);

            if (snake.is_point_on_body(food.position)) {
                onSnake = true;
            }
        } while(onSnake);
        score++;
    }
}

void World::handle_input(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (snake.direction != DOWN)
                    snake.direction = UP;
                break;
            case SDLK_DOWN:
                if (snake.direction != UP)
                    snake.direction = DOWN;
                break;
            case SDLK_LEFT:
                if (snake.direction != RIGHT)
                    snake.direction = LEFT;
                break;
            case SDLK_RIGHT:
                if (snake.direction != LEFT)
                    snake.direction = RIGHT;
                break;
            default:
                break;
        }
    }
}

void World::reset() {
    this->snake.body.clear();
    this->snake.body.push_front({25, 25});
    this->snake.body.push_back({24, 25});
    this->snake.body.push_back({23, 25});
    this->snake.direction = RIGHT;
    this->food.position = Point{10, 10};

    std::cout << "Score: " << this->score << std::endl;
    this->score = 0;
}

bool World::snake_is_eating_food() {
    return this->snake.body.front() == this->food.position;
}

bool World::snake_hit_wall() {
    Point head = this->snake.body.front();
    return head.x < 0 || head.x >= width / cell_size ||
           head.y < 0 || head.y >= height / cell_size;
}
