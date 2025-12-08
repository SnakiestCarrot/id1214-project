// in src/Game.cpp
#include "Game.hpp"
#include <iostream>

Game::Game()
    : window(nullptr), 
      renderer(nullptr), 
      m_shouldQuit(false), 
      m_visualizeToggled(false),
      m_menuToggled(false)
{
    
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "SDL shut down cleanly. Exiting." << std::endl;
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL initialized successfully!" << std::endl;

    window = SDL_CreateWindow("snake", 100, 100, 1002, 602, 0);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true; // Success
}

SDL_Renderer* Game::getRenderer() {
    return renderer;
}

void Game::prepareScene() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Game::presentScene() {
    SDL_RenderPresent(renderer);
}

void Game::pollEvents() {
    // Reset toggle flag each frame
    m_visualizeToggled = false;
    m_menuToggled = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_shouldQuit = true;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_v) {
                m_visualizeToggled = true;
            } else if( event.key.keysym.sym == SDLK_ESCAPE) {
                m_shouldQuit = true;
            } else if( event.key.keysym.sym == SDLK_m ) {
                m_menuToggled = true;
            }
        }
    }
}

bool Game::shouldQuit() const {
    return m_shouldQuit;
}

bool Game::visualizeToggled() const {
    return m_visualizeToggled;
}

bool Game::menuToggled() const {
    return m_menuToggled;
}