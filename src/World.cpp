
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "World.hpp"
#include "Snake.hpp"
#include "Food.hpp"

// Use an initializer list to store the reference
World::World(Snake& snake, Food& food) : snake(snake), food(food) {
    this->width = 1000;
    this->height = 600;
    this->cell_size = 20;
    this->score = 0;
    food.move_randomly(width / cell_size, height / cell_size);
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
    food.move_randomly(width / cell_size, height / cell_size);

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
    Point head = snake.body.front(); // head
    Point tail = snake.body.back(); // tail
    Direction dir = snake.direction; // heading
    Point food_pos = food.position; // food position

    // define relative points
    Point p_straight, p_left, p_right;
    switch (dir) {
        case UP:
            p_straight = {head.x, head.y - 1};
            p_left =     {head.x - 1, head.y};
            p_right =    {head.x + 1, head.y};
            break;
        case DOWN:
            p_straight = {head.x, head.y + 1};
            p_left =     {head.x + 1, head.y};
            p_right =    {head.x - 1, head.y};
            break;
        case LEFT:
            p_straight = {head.x - 1, head.y};
            p_left =     {head.x, head.y + 1};
            p_right =    {head.x, head.y - 1};
            break;
        case RIGHT:
            p_straight = {head.x + 1, head.y};
            p_left =     {head.x, head.y - 1};
            p_right =    {head.x, head.y + 1};
            break;
    }

    std::vector<double> inputs;

    // danger detection
    inputs.push_back(is_danger_at(p_left) ? 1.0 : 0.0);
    inputs.push_back(is_danger_at(p_straight) ? 1.0 : 0.0);
    inputs.push_back(is_danger_at(p_right) ? 1.0 : 0.0);

    // food direction
    bool food_left = false;
    bool food_right = false;
    bool food_up = false;
    bool food_down = false;
    if (food_pos.x < head.x) food_left = true;
    if (food_pos.x > head.x) food_right = true;
    if (food_pos.y < head.y) food_up = true;
    if (food_pos.y > head.y) food_down = true;

    // map food direction to relative directions based on current heading
    switch (dir) {
        case UP:
            inputs.push_back(food_left ? 1.0 : 0.0);  // Food is Left
            inputs.push_back(food_right ? 1.0 : 0.0); // Food is Right
            inputs.push_back(food_up ? 1.0 : 0.0);    // Food is Straight
            inputs.push_back(food_down ? 1.0 : 0.0);  // Food is Back
            break;
        case DOWN:
            inputs.push_back(food_right ? 1.0 : 0.0); // Food is Left
            inputs.push_back(food_left ? 1.0 : 0.0);  // Food is Right
            inputs.push_back(food_down ? 1.0 : 0.0);  // Food is Straight
            inputs.push_back(food_up ? 1.0 : 0.0);    // Food is Back
            break;
        case LEFT:
            inputs.push_back(food_down ? 1.0 : 0.0);  // Food is Left
            inputs.push_back(food_up ? 1.0 : 0.0);    // Food is Right
            inputs.push_back(food_left ? 1.0 : 0.0);  // Food is Straight
            inputs.push_back(food_right ? 1.0 : 0.0); // Food is Back
            break;
        case RIGHT:
            inputs.push_back(food_up ? 1.0 : 0.0);    // Food is Left
            inputs.push_back(food_down ? 1.0 : 0.0);  // Food is Right
            inputs.push_back(food_right ? 1.0 : 0.0); // Food is Straight
            inputs.push_back(food_left ? 1.0 : 0.0);  // Food is Back
            break;
    }

    // tail direction
    inputs.push_back(tail.x < head.x ? 1.0 : 0.0); // Tail is Left
    inputs.push_back(tail.x > head.x ? 1.0 : 0.0); // Tail is Right
    inputs.push_back(tail.y < head.y ? 1.0 : 0.0); // Tail is Up
    inputs.push_back(tail.y > head.y ? 1.0 : 0.0); // Tail is Down

    return inputs; // Final 11 inputs
}

void World::handle_ai_input(NeuralNetwork& brain) {
    // 1. Get the current "senses" from the game world
    std::vector<double> inputs = get_game_state();

    // 2. Feed them to the neural network
    std::vector<double> outputs = brain.feedForward(inputs);

    // 3. Interpret the network's decision
    // Find the index of the highest output value.
    // 0 = Turn Left, 1 = Go Straight, 2 = Turn Right
    auto max_it = std::max_element(outputs.begin(), outputs.end());
    int decision = std::distance(outputs.begin(), max_it);

    Direction current_dir = snake.direction;
    Direction new_dir = current_dir;

    switch (decision) {
        case 0: // --- Turn Left (Relative)
            if (current_dir == UP) new_dir = LEFT;
            else if (current_dir == LEFT) new_dir = DOWN;
            else if (current_dir == DOWN) new_dir = RIGHT;
            else if (current_dir == RIGHT) new_dir = UP;
            break;
        
        case 1: // --- Go Straight
            // No change
            break;

        case 2: // --- Turn Right (Relative)
            if (current_dir == UP) new_dir = RIGHT;
            else if (current_dir == RIGHT) new_dir = DOWN;
            else if (current_dir == DOWN) new_dir = LEFT;
            else if (current_dir == LEFT) new_dir = UP;
            break;
    }
    
    snake.direction = new_dir;
}

bool World::is_danger_at(Point p) {
    // wall collision
    if (p.x < 0 || p.x >= (width / cell_size) ||
        p.y < 0 || p.y >= (height / cell_size)) {
        return true;
    }
    
    // self collision
    return snake.is_point_on_body(p, true); 
}

int World::getScore() const {
    return this->score;
}
