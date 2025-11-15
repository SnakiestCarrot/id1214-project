#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "Snake.hpp"
#include "World.hpp"
#include "Food.hpp"
#include "NeuralNetwork.hpp" // <-- 1. Include the Neural Network

#define FPS 15

const int START_SCREEN_X = 100;
const int START_SCREEN_Y = 100;

const int CELL_SIZE = 20;

const int SCREEN_WIDTH = 1002;
const int SCREEN_HEIGHT = 602;

SDL_Window * init_window();
SDL_Renderer * init_renderer(SDL_Window *window);

// 2. Topology for the AI brain (6 inputs, 10 hidden, 4 outputs)
std::vector<size_t> topology = {6, 10, 4};

int main(int argc, char* argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    SDL_Window *window = init_window();
    if (!window) {
        std::cerr << "Failed to create window: " << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = init_renderer(window);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << std::endl;
        return 1;
    }

    Snake snake = Snake();
    Food food = Food(10, 10);
    World world = World(snake, food); 

    // <-- 3. Create the AI's "brain"
    NeuralNetwork brain(topology, ActivationType::RELU);
    // (Later, you can load saved genes here: brain.setGenes(...))

    bool running = true;
    // <-- 4. Add a toggle for AI control (start with human)
    bool ai_control_enabled = false; 
    std::cout << "Starting in Manual (human) mode. Press 'M' to toggle AI." << std::endl;

    while (running) {
        SDL_Event event;

        // <-- 5. AI Input (runs *before* polling if enabled)
        // The AI makes a decision every single frame
        if (ai_control_enabled) {
            world.handle_ai_input(brain);
        }

        // <-- 6. Event Polling (handles quit, human input, AI toggle)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Close window event received." << std::endl;
                running = false;
            }

            // Check for the AI toggle key
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) {
                ai_control_enabled = !ai_control_enabled;
                std::cout << "AI Control: " << (ai_control_enabled ? "ON" : "OFF") << std::endl;
            }

            // Only listen for human input if AI is OFF
            if (!ai_control_enabled) {
                world.handle_input(event);
            }
        }
        
        // --- Game Logic and Rendering ---
        world.update();
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        world.draw(renderer);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(1000/FPS);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "SDL shut down cleanly. Exiting." << std::endl;
    return 0;
}

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