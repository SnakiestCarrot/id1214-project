#pragma once

#include <vector>
#include <string>
#include <memory>         // <-- NEW: For std::unique_ptr
#include "Population.hpp"
#include "Game.hpp" 
#include "Snake.hpp"      // <-- NEW
#include "Food.hpp"       // <-- NEW
#include "World.hpp"      // <-- NEW

class Trainer {
public:
    Trainer();
    ~Trainer(); 
    void run();

private:
    void run_training_generation();
    void run_visualization();
    
    void start_visualization(); 
    void end_visualization();

    const size_t POPULATION_SIZE;
    const std::vector<size_t> topology;
    int FPS = 80;
    const int MAX_STEPS_PER_GAME = 10000;

    Game game; 
    Population population; 
    
    std::unique_ptr<Snake> vis_snake;
    std::unique_ptr<Food>  vis_food;
    std::unique_ptr<World> vis_world;
    
    bool visualize_mode;
};