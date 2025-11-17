
#pragma once

#include <SDL2/SDL.h>
#include <string>

class Game {
public:
    Game();
    ~Game();

    bool init();
    
    void pollEvents();
    bool shouldQuit() const;
    bool visualizeToggled() const;

    void prepareScene();
    void presentScene();
    SDL_Renderer* getRenderer();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool m_shouldQuit;
    bool m_visualizeToggled;
};
