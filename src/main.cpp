#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "Snake.hpp"
#include "World.hpp"
#include "Food.hpp"

#define FPS 15

const int START_SCREEN_X = 100;
const int START_SCREEN_Y = 100;

const int CELL_SIZE = 20;

const int SCREEN_WIDTH = 1002;
const int SCREEN_HEIGHT = 602;

SDL_Window * init_window();
SDL_Renderer * init_renderer(SDL_Window *window);

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

    if(!renderer) {
        std::cerr << "Failed to create renderer: " << std::endl;
        return 1;
    }

    Snake snake = Snake();
    Food food = Food(10, 10);

    World world = World(snake, food); 

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Close window event received." << std::endl;
                running = false;
            }

            world.handle_input(event);
        }
        
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
        "snake",
        START_SCREEN_X,
        START_SCREEN_Y,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
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