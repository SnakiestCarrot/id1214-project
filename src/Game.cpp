#include "Game.hpp"

Game::Game() : m_window(nullptr), m_renderer(nullptr), m_isRunning(false) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL failed: " << SDL_GetError() << std::endl;
        return false;
    }
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!m_window) return false;

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) return false;

    m_isRunning = true;
    return true;
}

bool Game::pollEvent(SDL_Event& event) {
    return SDL_PollEvent(&event);
}

void Game::clear() {
    SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 255);
    SDL_RenderClear(m_renderer);
}

void Game::present() {
    SDL_RenderPresent(m_renderer);
}

void Game::clean() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
