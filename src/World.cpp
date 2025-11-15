
#include <iostream>
#include <algorithm>
#include <iterator>

#include "World.hpp"
#include "Snake.hpp"
#include "Food.hpp"

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

            if (snake.is_point_on_body(food.position, false)) {
                onSnake = true;
            }
        } while(onSnake);
        score++;
    }
}

void World::handle_input(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w: 
                if (snake.direction != DOWN)
                    snake.direction = UP;
                break;
            case SDLK_s: 
                if (snake.direction != UP)
                    snake.direction = DOWN;
                break;
            case SDLK_a: 
                if (snake.direction != RIGHT)
                    snake.direction = LEFT;
                break;
            case SDLK_d: 
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

std::vector<double> World::get_game_state() {
    Point head = snake.body.front();

    // 1. Define the 4 absolute points to check for danger
    Point p_up = {head.x, head.y - 1};
    Point p_down = {head.x, head.y + 1};
    Point p_left = {head.x - 1, head.y};
    Point p_right = {head.x + 1, head.y};

    // 2. Create the input vector
    std::vector<double> inputs;

    // Inputs 0-3: Danger in 4 absolute directions (1.0 = danger, 0.0 = safe)
    inputs.push_back(is_danger_at(p_up) ? 1.0 : 0.0);
    inputs.push_back(is_danger_at(p_down) ? 1.0 : 0.0);
    inputs.push_back(is_danger_at(p_left) ? 1.0 : 0.0);
    inputs.push_back(is_danger_at(p_right) ? 1.0 : 0.0);

    // Inputs 4, 5: Food Direction (Normalized: -1, 0, or 1)
    Point food_pos = food.position;
    
    // X-direction to food
    if (food_pos.x < head.x) inputs.push_back(-1.0); // Food is to the left
    else if (food_pos.x > head.x) inputs.push_back(1.0); // Food is to the right
    else inputs.push_back(0.0); // Food is on the same column

    // Y-direction to food
    if (food_pos.y < head.y) inputs.push_back(-1.0); // Food is above
    else if (food_pos.y > head.y) inputs.push_back(1.0); // Food is below
    else inputs.push_back(0.0); // Food is on the same row

    return inputs;
}

void World::handle_ai_input(NeuralNetwork& brain) {
    std::vector<double> inputs = get_game_state();

    std::vector<double> outputs = brain.feedForward(inputs);

    auto max_it = std::max_element(outputs.begin(), outputs.end());
    int decision = std::distance(outputs.begin(), max_it);

    switch (decision) {
        case 0: // Output 0 -> UP
            snake.direction = UP;
            break;
        case 1: // Output 1 -> DOWN
            snake.direction = DOWN;
            break;
        case 2: // Output 2 -> LEFT
            snake.direction = LEFT;
            break;
        case 3: // Output 3 -> RIGHT
            snake.direction = RIGHT;
            break;
    }
}

bool World::is_danger_at(Point p) {
    // 1. Check for wall collision
    if (p.x < 0 || p.x >= (width / cell_size) ||
        p.y < 0 || p.y >= (height / cell_size)) {
        return true;
    }
    
    // 2. Check for self-collision
    // This relies on your snake.is_point_on_body function
    // being able to skip the tail.
    return snake.is_point_on_body(p, true); 
}
