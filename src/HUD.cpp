#include "HUD.h"
#include <iostream>

// Định nghĩa constructor ở đây
HUD::HUD(SDL_Renderer* gRenderer, TTF_Font* gFont, GameOver* gGameOver)
    : renderer(gRenderer), font(gFont), gameOver(gGameOver) {}

HUD::~HUD() {}

void HUD::render() {
    SDL_Color white = {255, 255, 255};

    // Hiển thị health
    std::string healthText = "Health: " + std::to_string(gameOver->getHealth());
    renderText(healthText, 10, 10, white);

    // Hiển thị score
    std::string scoreText = "Score: " + std::to_string(gameOver->getScore());
    renderText(scoreText, 10, 50, white);
}

void HUD::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
