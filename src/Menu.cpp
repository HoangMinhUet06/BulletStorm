
#include "Menu.h"
#include <SDL_image.h>
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer) {
    TTF_Init();
    font = TTF_OpenFont("assets/font.ttf", 48); // Chỉnh font theo mong muốn

    // Tạo các button menu
    playButton.text = "PLAY";
    tutorialButton.text = "TUTORIAL";
    settingButton.text = "SETTING";

    playButton.rect = {300, 200, 200, 50};
    tutorialButton.rect = {300, 300, 250, 50};
    settingButton.rect = {300, 400, 220, 50};

    // Tạo texture chữ
    playButton.textureNormal = createTextTexture(playButton.text, normalColor);
    playButton.textureHover = createTextTexture(playButton.text, hoverColor);

    tutorialButton.textureNormal = createTextTexture(tutorialButton.text, normalColor);
    tutorialButton.textureHover = createTextTexture(tutorialButton.text, hoverColor);

    settingButton.textureNormal = createTextTexture(settingButton.text, normalColor);
    settingButton.textureHover = createTextTexture(settingButton.text, hoverColor);
}

Menu::~Menu() {
    TTF_CloseFont(font);
    TTF_Quit();
}

// Hàm tạo texture chữ từ chuỗi text
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

    // Kiểm tra nếu chuột đang hover vào nút
    if (SDL_PointInRect( new SDL_Point{mouseX, mouseY}, &playButton.rect)) {
        selectedOption = PLAY;
    } else if (SDL_PointInRect( new SDL_Point{mouseX, mouseY}, &tutorialButton.rect)) {
        selectedOption = TUTORIAL;
    } else if (SDL_PointInRect( new SDL_Point{mouseX, mouseY}, &settingButton.rect)) {
        selectedOption = SETTING;
    }

    // Nếu nhấn chuột vào mục nào thì in ra hành động tương ứng
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (selectedOption == PLAY) {
            std::cout << "Play game!\n";
        } else if (selectedOption == TUTORIAL) {
            std::cout << "Show tutorial!\n";
        } else if (selectedOption == SETTING) {
            std::cout << "Open settings!\n";
        }
    }
}

void Menu::render() {
    // Hiển thị các mục menu với màu thay đổi khi hover
    SDL_RenderCopy(renderer, (selectedOption == PLAY) ? playButton.textureHover : playButton.textureNormal, NULL, &playButton.rect);
    SDL_RenderCopy(renderer, (selectedOption == TUTORIAL) ? tutorialButton.textureHover : tutorialButton.textureNormal, NULL, &tutorialButton.rect);
    SDL_RenderCopy(renderer, (selectedOption == SETTING) ? settingButton.textureHover : settingButton.textureNormal, NULL, &settingButton.rect);
}

MenuOption Menu::getSelectedOption() {
    return static_cast<MenuOption>(selectedOption);
}
