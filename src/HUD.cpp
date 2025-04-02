#include "HUD.h"
#include <iostream>

HUD::HUD(SDL_Renderer* gRenderer, TTF_Font* gFont)
    : renderer(gRenderer), font(gFont), health(3), score(0) {}

HUD::~HUD() {}

void HUD::updateHealth(int newHealth) {
    health = newHealth;
}

void HUD::updateScore(int newScore) {
    score = newScore;
}

void HUD::render() {
    // Màu sắc của thanh máu và điểm
    SDL_Color healthColor = {255, 0, 0, 255};  // Màu đỏ cho thanh máu
    SDL_Color scoreColor = {255, 255, 255, 255};  // Màu trắng cho điểm

    // Vẽ thanh máu
    renderText("HP: " + std::to_string(health), 10, 10, healthColor);

    // Vẽ điểm số
    renderText("Score: " + std::to_string(score), 10, 50, scoreColor);
}

void HUD::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
