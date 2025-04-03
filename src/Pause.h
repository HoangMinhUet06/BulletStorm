#ifndef PAUSE_H
#define PAUSE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include "GameState.h"

class PauseMenu {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* resumeText;
    SDL_Texture* quitText;
    SDL_Rect resumeRect;
    SDL_Rect quitRect;
    SDL_Texture* pauseBackground;

    bool isPaused;
    int selectedOption; // 0: Resume, 1: Quit

    void handleSelection(bool& quit, bool inMenu, GameState& gameState);

public:
    PauseMenu(SDL_Renderer* renderer);
    ~PauseMenu();

    void handleEvent(SDL_Event& e, bool& quit, bool inMenu, GameState& gameState);
    void render();
    bool getPaused() const;
    void togglePause();
    SDL_Texture* renderText(const std::string& text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer);
};

#endif
