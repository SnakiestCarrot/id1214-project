
#include <iostream>
#include <SDL2/SDL.h>
#include "snake.hpp"

const int START_SCREEN_X = 100;
const int START_SCREEN_Y = 100;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "Hello!" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
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

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL, 0);

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    Snake snake = Snake();

    bool running = true;
    while (running) {
        SDL_Event event;
        // Poll for events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Close window event received." << std::endl;
                running = false;
            }
        }

        

        snake.update();
        
        SDL_Delay(1000);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "SDL shut down cleanly. Exiting." << std::endl;
    return 0;
}