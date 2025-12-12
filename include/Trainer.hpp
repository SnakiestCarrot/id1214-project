#pragma once

#include <vector>
#include <memory>
#include <future>
#include "Game.hpp" 
#include "Population.hpp"
#include "World.hpp"

enum class TrainerState {
    Menu,
    Training,
    Visualizing
};

class Trainer {
public:
    Trainer();
    ~Trainer(); 
    void run();

private:
    void handleEvents();
    void update();
    void render();

    void startVisualization(); 
    void stopVisualization();
    void runTrainingStep();
    void runVisualizationStep();
    
    void resetTraining();

    void renderGraph(SDL_Renderer* renderer, int x, int y, int w, int h);

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    const size_t POPULATION_SIZE;
    std::vector<size_t> m_topology;
    size_t m_hiddenNodeCount;
    const int FPS = 120;
    const int MAX_STEPS_PER_GAME = 2500;

    Game m_game; 
    Population m_population;
    TrainerState m_state;

    std::vector<double> m_fitnessHistory;

    SDL_Rect btnReset;
    SDL_Rect btnMinusNode;
    SDL_Rect btnPlusNode;

    std::unique_ptr<Snake> m_visSnake;
    std::unique_ptr<Food>  m_visFood;
    std::unique_ptr<World> m_visWorld;
};