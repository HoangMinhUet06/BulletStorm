#include "Menu.h"
#include <SDL_image.h>
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer) {
    TTF_Init();
    font = TTF_OpenFont("C:/CodeBlock/BulletStorm/assets/font/pixel_font.ttf", 24);

    playButton.text = "PLAY";

    playButton.rect = {300, 200, 200, 50};

    playButton.textureNormal = createTextTexture(playButton.text, normalColor);
    playButton.textureHover = createTextTexture(playButton.text, hoverColor);

}

Menu::~Menu() {
    TTF_CloseFont(font);
    TTF_Quit();
}

SDL_Texture* Menu::createTextTexture(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Menu::handleEvent(SDL_Event& e) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    selectedOption = NONE;

    if (SDL_PointInRect( new SDL_Point{mouseX, mouseY}, &playButton.rect)) {
        selectedOption = PLAY;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (selectedOption == PLAY) {
            std::cout << "Play game!\n";
        }
    }
}

void Menu::render() {
    SDL_RenderCopy(renderer, (selectedOption == PLAY) ? playButton.textureHover : playButton.textureNormal, NULL, &playButton.rect);

}

MenuOption Menu::getSelectedOption() {
    return static_cast<MenuOption>(selectedOption);
}
