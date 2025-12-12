#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    
    bool pollEvent(SDL_Event& event);
    void clear();
    void present();
    void clean();

    SDL_Renderer* getRenderer() const { return m_renderer; }
    bool isRunning() const { return m_isRunning; }
    void stop() { m_isRunning = false; }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_isRunning;
};