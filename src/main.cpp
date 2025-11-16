#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <thread>
#include <future>
#include <cmath>
#include <algorithm>

#include "Snake.hpp"
#include "World.hpp"
#include "Food.hpp"
#include "NeuralNetwork.hpp"
#include "Population.hpp"

// --- SDL Settings ---
#define FPS 80
const int START_SCREEN_X = 100;
const int START_SCREEN_Y = 100;
const int CELL_SIZE = 20;
const int SCREEN_WIDTH = 1002;
const int SCREEN_HEIGHT = 602;

// --- GA Settings ---
const size_t POPULATION_SIZE = 500;
std::vector<size_t> topology = {11, 20, 12, 3};
const int MAX_STEPS_PER_GAME = 10000; // Prevents infinite loops

// --- Function Declarations ---
SDL_Window * init_window();
SDL_Renderer * init_renderer(SDL_Window *window);


/**
 * @brief
 * Plays one full game "headless" for a single brain.
 * This is designed to be run on a separate thread.
 */
double evaluate_brain_fitness(const std::vector<size_t>& topology, const std::vector<double>& genes) {
    
    // Each thread gets its OWN set of game objects
    Snake snake;
    Food food(10, 10);
    World world(snake, food);
    
    // Re-create the brain from its genes
    NeuralNetwork brain(topology, ActivationType::RELU);
    brain.setGenes(genes);

    int steps = 0;
    int score_at_last_food = 0;
    int steps_since_last_food = 0;

    while (!world.snake_hit_wall() && !snake.hit_itself() && steps < MAX_STEPS_PER_GAME) {
        
        world.handle_ai_input(brain);
        world.update();

        steps++;
        steps_since_last_food++;

        if (world.getScore() > score_at_last_food) {
            score_at_last_food = world.getScore();
            steps_since_last_food = 0;
        }
        if (steps_since_last_food > 150) {
            // Snake is stuck in a loop, kill it
            break;
        }
    }

    double fitness = (double)steps + (double)(world.getScore() * 800);
    return fitness;
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    SDL_Window *window = init_window();
    SDL_Renderer *renderer = init_renderer(window);
    if (!window || !renderer) return 1;

    Population population(POPULATION_SIZE, topology);

    bool running = true;
    bool visualize_mode = false;
    
    std::cout << "Starting in Training Mode. Press 'V' to visualize." << std::endl;

    unsigned int num_threads = std::thread::hardware_concurrency();
    std::cout << "Running on " << num_threads << " threads." << std::endl;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_v) {
                    visualize_mode = !visualize_mode;
                    std::cout << "Mode set to: " 
                              << (visualize_mode ? "Visualize" : "Training") 
                              << std::endl;
                }
            }
        }

        if (visualize_mode) {
            
            Snake snake;
            Food food(10, 10);
            World world(snake, food);
            NeuralNetwork& bestBrain = population.getBrain(0);

            while (!world.snake_hit_wall() && !snake.hit_itself()) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) running = false;
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_v) {
                        visualize_mode = false;
                        break;
                    }
                }
                if (!running || !visualize_mode) break;

                world.handle_ai_input(bestBrain);
                world.update();

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                world.draw(renderer);
                SDL_RenderPresent(renderer);

                SDL_Delay(1000 / FPS);
            }

        } else {
            // vector to hold our "promised" fitness results
            std::vector< std::future<double> > fitness_futures;

            // launch all jobs in parallel
            for (size_t i = 0; i < population.size(); ++i) {
                std::vector<double> genes = population.getBrain(i).getGenes();

                fitness_futures.push_back(
                    std::async(std::launch::async, evaluate_brain_fitness, topology, genes)
                );
            }

            for (size_t i = 0; i < population.size(); ++i) {
                // blocks until the future is ready
                double fitness = fitness_futures[i].get();
                population.setFitness(i, fitness);
            }

            population.evolve();
            
            std::cout << "Generation: " << population.getGeneration() 
                      << " | Best Fitness: " << population.getBestFitness() 
                      << std::endl;
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    std::cout << "SDL shut down cleanly. Exiting." << std::endl;
    return 0;
}


// --- SDL Helper Functions (no changes) ---

SDL_Window * init_window() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return NULL;
    }
    std::cout << "SDL initialized successfully!" << std::endl;
    SDL_Window* window = SDL_CreateWindow(
        "snake", START_SCREEN_X, START_SCREEN_Y,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0
    );
    return window;
}

SDL_Renderer * init_renderer(SDL_Window *window) {
    if (!window) return NULL; // Check if window failed
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return renderer;
}