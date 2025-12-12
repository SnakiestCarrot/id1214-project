#include "Trainer.hpp"
#include "NeuralNetwork.hpp"
#include <iostream>
#include <thread>
#include <future>

double evaluate_brain_fitness(const std::vector<size_t>& topology, const std::vector<double>& genes, int max_steps, int width, int height) {
    Snake snake;
    Food food(10, 10);
    World world(snake, food, width, height); 
    NeuralNetwork brain(topology, ActivationType::RELU); 
    brain.setGenes(genes);
    int steps = 0;
    int score_at_last_food = 0;
    int steps_since_last_food = 0;
    while (!world.snake_hit_wall() && !snake.hit_itself() && steps < max_steps) {
        world.handle_ai_input(brain);
        world.update();
        steps++;
        steps_since_last_food++;
        if (world.getScore() > score_at_last_food) {
            score_at_last_food = world.getScore();
            steps_since_last_food = 0;
        }
        if (steps_since_last_food > 150) break;
    }
    return (double)steps + (double)(world.getScore() * 1000.0);
}

const int INPUT_NODES = 11;
const int DEFAULT_HIDDEN_NODES = 8;
const int OUTPUT_NODES = 3;

Trainer::Trainer()
    : POPULATION_SIZE(500),
      m_topology{INPUT_NODES, DEFAULT_HIDDEN_NODES, OUTPUT_NODES},
      m_hiddenNodeCount(8),
      m_game(), 
      m_population(POPULATION_SIZE, m_topology),
      m_state(TrainerState::Menu)
{
    if (!m_game.init("AI Snake Trainer", WINDOW_WIDTH, WINDOW_HEIGHT)) {
        std::cerr << "Game Init Failed" << std::endl;
        exit(-1);
    }

    btnReset = { 350, 200, 100, 50 }; 

    btnMinusNode = { 300, 300, 50, 50 };

    btnPlusNode = { 450, 300, 50, 50 };

    std::cout << "Trainer Initialized." << std::endl;
}

Trainer::~Trainer() {}

void Trainer::run() {
    while (m_game.isRunning()) {
        handleEvents();
        update();
        render();
    }
}

void Trainer::handleEvents() {
    SDL_Event event;
    while (m_game.pollEvent(event)) {
        if (event.type == SDL_QUIT) {
            m_game.stop();
            return;
        }

        if (m_state == TrainerState::Menu && event.type == SDL_MOUSEBUTTONDOWN) {
            int mx = event.button.x;
            int my = event.button.y;
            SDL_Point mousePoint = {mx, my};

            if (SDL_PointInRect(&mousePoint, &btnReset)) {
                std::cout << "Button: Reset Logic" << std::endl;
                resetTraining();
            }

            if (SDL_PointInRect(&mousePoint, &btnMinusNode)) {
                if (m_hiddenNodeCount > 0) {
                    m_hiddenNodeCount--;
                    std::cout << "Nodes Decreased to " << m_hiddenNodeCount << std::endl;
                    resetTraining();
                }
            }

            if (SDL_PointInRect(&mousePoint, &btnPlusNode)) {
                m_hiddenNodeCount++;
                std::cout << "Nodes Increased to " << m_hiddenNodeCount << std::endl;
                resetTraining();
            }
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    if (m_state == TrainerState::Visualizing) {
                        stopVisualization();
                        m_state = TrainerState::Menu;
                    } else {
                        m_game.stop();
                    }
                    break;
                case SDLK_m: 
                    if (m_state == TrainerState::Visualizing) stopVisualization();
                    m_state = TrainerState::Menu;
                    break;
                case SDLK_t: 
                    if (m_state == TrainerState::Visualizing) stopVisualization();
                    m_state = TrainerState::Training;
                    break;
                case SDLK_v: 
                    if (m_state != TrainerState::Visualizing) {
                        m_state = TrainerState::Visualizing;
                        startVisualization();
                    }
                    break;
            }
            if (event.key.keysym.sym == SDLK_m) {
                 m_state = TrainerState::Menu;
                 if(m_visWorld) stopVisualization();
             }
             else if (event.key.keysym.sym == SDLK_t) m_state = TrainerState::Training;
             else if (event.key.keysym.sym == SDLK_v) {
                 m_state = TrainerState::Visualizing;
                 startVisualization();
             }
             else if (event.key.keysym.sym == SDLK_ESCAPE) m_game.stop();
        }
    }
}

void Trainer::resetTraining() {
    m_topology.clear();
    m_topology.push_back(INPUT_NODES); 
    if (m_hiddenNodeCount > 0) {
        m_topology.push_back(m_hiddenNodeCount);
    }

    m_topology.push_back(OUTPUT_NODES);
    m_population.reset(POPULATION_SIZE, m_topology);
    m_fitnessHistory.clear();
}

void Trainer::update() {
    switch (m_state) {
        case TrainerState::Menu:
            break;
        case TrainerState::Training:
            runTrainingStep();
            break;
        case TrainerState::Visualizing:
            runVisualizationStep();
            break;
    }
}

void Trainer::render() {
    m_game.clear(); 

    switch (m_state) {
        case TrainerState::Menu: {
            SDL_Renderer* r = m_game.getRenderer();
            
            SDL_SetRenderDrawColor(r, 200, 50, 50, 255);
            SDL_RenderFillRect(r, &btnReset);

            SDL_SetRenderDrawColor(r, 50, 50, 200, 255);
            SDL_RenderFillRect(r, &btnMinusNode);

            SDL_SetRenderDrawColor(r, 50, 200, 50, 255);
            SDL_RenderFillRect(r, &btnPlusNode);
            
            SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            
            if (m_hiddenNodeCount == 0) {
                 SDL_Rect line = { 360, 325, 20, 2 };
                 SDL_RenderFillRect(r, &line);
            } else {
                for(size_t i=0; i < m_hiddenNodeCount; ++i) {
                    SDL_Rect nodeRect = { 360 + ((int)i * 10), 310, 5, 30 }; 
                    SDL_RenderFillRect(r, &nodeRect);
                }
            }
            break;
        }

        case TrainerState::Training:
            renderGraph(m_game.getRenderer(), 0, 0, 800, 600);
            break;

        case TrainerState::Visualizing:
            if (m_visWorld) {
                m_visWorld->draw(m_game.getRenderer());
            }
            break;
    }
    if(m_state == TrainerState::Visualizing && m_visWorld) m_visWorld->draw(m_game.getRenderer());
    m_game.present();
}

void Trainer::startVisualization() {
    m_visSnake = std::make_unique<Snake>();
    m_visFood  = std::make_unique<Food>(10, 10);
    m_visWorld = std::make_unique<World>(*m_visSnake, *m_visFood, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Trainer::stopVisualization() {
    m_visWorld.reset();
    m_visFood.reset();
    m_visSnake.reset();
}

void Trainer::runVisualizationStep() {
    if (!m_visWorld) return;
    if (m_visWorld->snake_hit_wall() || m_visSnake->hit_itself()) {
        startVisualization(); 
        return;
    }
    NeuralNetwork& bestBrain = m_population.getBrain(0); 
    m_visWorld->handle_ai_input(bestBrain);
    m_visWorld->update();
    SDL_Delay(1000 / FPS);
}

void Trainer::runTrainingStep() {
    std::vector<std::future<double>> futures;
    futures.reserve(m_population.size());

    for (size_t i = 0; i < m_population.size(); ++i) {
        std::vector<double> genes = m_population.getBrain(i).getGenes();
        
        futures.push_back(
            std::async(std::launch::async, evaluate_brain_fitness, m_topology, genes, MAX_STEPS_PER_GAME, WINDOW_WIDTH, WINDOW_HEIGHT)
        );
    }
    
    for (size_t i = 0; i < m_population.size(); ++i) {
        m_population.setFitness(i, futures[i].get());
    }

    double avgFitness = m_population.getAverageFitness();
    m_fitnessHistory.push_back(avgFitness);

    if (m_population.getGeneration() % 1 == 0) {
         std::cout << "Gen: " << m_population.getGeneration() 
                  << " | Best: " << (int)m_population.getBestFitness()
                  << " | Avg: " << avgFitness
                  << " | Topology: " << m_hiddenNodeCount << " hidden nodes" << std::endl;
    }
    m_population.evolve();
}

void Trainer::renderGraph(SDL_Renderer* renderer, int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_Rect bg = {x, y, w, h};
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bg);

    if (m_fitnessHistory.empty()) return;

    double maxVal = 0.0;
    for (double f : m_fitnessHistory) {
        if (f > maxVal) maxVal = f;
    }
    if (maxVal < 1.0) maxVal = 1.0;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    
    int numPoints = m_fitnessHistory.size();
    if (numPoints < 2) return;

    double xStep = (double)w / (double)(numPoints - 1);

    for (int i = 0; i < numPoints - 1; ++i) {
        int x1 = x + (int)(i * xStep);
        int x2 = x + (int)((i + 1) * xStep);
        
        int y1 = (y + h) - (int)((m_fitnessHistory[i] / maxVal) * h);
        int y2 = (y + h) - (int)((m_fitnessHistory[i+1] / maxVal) * h);

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}
