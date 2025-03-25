
#include "Game.h"
#include <SDL_image.h>

Game::Game(SDL_Renderer* renderer) : renderer(renderer), background(nullptr), character(nullptr) {}

Game::~Game() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(character);
}

void Game::loadResources() {
    // Load background
    SDL_Surface* bgSurface = IMG_Load("C:/CodeBlock/BulletStorm/assets/background.png");
    background = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Load nhân vật
    SDL_Surface* charSurface = IMG_Load("C:/CodeBlock/BulletStorm/assets/character.png");
    character = SDL_CreateTextureFromSurface(renderer, charSurface);
    SDL_FreeSurface(charSurface);

    // Định vị trí nhân vật ở góc trái màn hình
    characterRect = {50, 450, 50, 50};
}

void Game::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                characterRect.x -= 10;
                break;
            case SDLK_RIGHT:
                characterRect.x += 10;
                break;
            case SDLK_UP:
                characterRect.y -= 10;
                break;
            case SDLK_DOWN:
                characterRect.y += 10;
                break;
        }
    }
}

void Game::update() {
    // Cập nhật logic game nếu cần
}

void Game::render() {
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_RenderCopy(renderer, character, NULL, &characterRect);
}
