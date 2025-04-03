#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "GameOver.h"
#include<string>

class HUD {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameOver* gameOver;

public:

    HUD(SDL_Renderer* gRenderer, TTF_Font* gFont, GameOver* gGameOver);
    ~HUD();
    void render();
    void renderText(const std::string& text, int x, int y, SDL_Color color);
};

#endif
