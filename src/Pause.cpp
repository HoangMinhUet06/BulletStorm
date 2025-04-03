#include "Pause.h"
#include <SDL_ttf.h>
#include <iostream>

PauseMenu::PauseMenu(SDL_Renderer* renderer)
    : renderer(renderer), isPaused(false), selectedOption(0) {
    // Khởi tạo font
    TTF_Init();
    font = TTF_OpenFont("C:/BulletStorm/BulletStormX/assets/font/pixel_font.ttf", 24);
    if (!font) {
        std::cerr << "Không thể load font: " << TTF_GetError() << std::endl;
    }

    // Tạo texture chữ
    SDL_Color white = {255, 255, 255};
    SDL_Surface* resumeSurface = TTF_RenderText_Solid(font, "Resume", white);
    SDL_Surface* quitSurface = TTF_RenderText_Solid(font, "Quit", white);

    resumeText = SDL_CreateTextureFromSurface(renderer, resumeSurface);
    quitText = SDL_CreateTextureFromSurface(renderer, quitSurface);

    SDL_FreeSurface(resumeSurface);
    SDL_FreeSurface(quitSurface);

    // Vị trí của text
    resumeRect = {400, 250, 200, 50};
    quitRect = {400, 320, 200, 50};

    // Tải nền Pause
    pauseBackground = IMG_LoadTexture(renderer, "C:/BulletStorm/BulletStormX/assets/images/start_background.jpg");
    if (!pauseBackground) {
        std::cerr << "Không thể tải ảnh nền pause: " << IMG_GetError() << std::endl;
    }
}

PauseMenu::~PauseMenu() {
    if (pauseBackground) {
        SDL_DestroyTexture(pauseBackground);
    }
    SDL_DestroyTexture(resumeText);
    SDL_DestroyTexture(quitText);
    TTF_CloseFont(font);
    TTF_Quit();
}

void PauseMenu::handleEvent(SDL_Event& e, bool& quit, bool inMenu, GameState& gameState) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_p) {
            togglePause();
            return;
        }
    }

    if (!isPaused) return;

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                selectedOption = 0;
                break;
            case SDLK_DOWN:
                selectedOption = 1;
                break;
            case SDLK_RETURN:
                handleSelection(quit, inMenu, gameState);
                break;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= resumeRect.x && mouseX <= resumeRect.x + resumeRect.w &&
            mouseY >= resumeRect.y && mouseY <= resumeRect.y + resumeRect.h) {
            selectedOption = 0;
            handleSelection(quit, inMenu, gameState);
        }
        else if (mouseX >= quitRect.x && mouseX <= quitRect.x + quitRect.w &&
                 mouseY >= quitRect.y && mouseY <= quitRect.y + quitRect.h) {
            selectedOption = 1;
            handleSelection(quit, inMenu, gameState);
        }
    }
}

void PauseMenu::handleSelection(bool& quit, bool inMenu, GameState& gameState) {
    if (selectedOption == 0) { // Resume
        togglePause();
    } else { // Quit
        isPaused = false;
        gameState = MENU_STATE;  // Chuyển về menu chính
    }
}

void PauseMenu::render() {
    if (!isPaused) return;

    // Hiển thị ảnh nền Pause
    SDL_Rect bgRect = {0, 0, 1024, 640}; // Kích thước màn hình
    SDL_RenderCopy(renderer, pauseBackground, nullptr, &bgRect);

    // Màu chữ
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0}; // Màu vàng cho mục đang chọn

    // Vẽ chữ Resume
    SDL_Texture* resumeText = renderText("Resume", (selectedOption == 0) ? yellow : white, font, renderer);
    SDL_Rect resumeRect = {400, 250, 200, 60};
    SDL_RenderCopy(renderer, resumeText, nullptr, &resumeRect);

    // Vẽ chữ Quit
    SDL_Texture* quitText = renderText("Quit", (selectedOption == 1) ? yellow : white, font, renderer);
    SDL_Rect quitRect = {400, 320, 200, 60};
    SDL_RenderCopy(renderer, quitText, nullptr, &quitRect);
}

void PauseMenu::togglePause() {
    isPaused = !isPaused;
}

bool PauseMenu::getPaused() const {
    return isPaused;
}

SDL_Texture* PauseMenu::renderText(const std::string& text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Không thể tạo surface từ văn bản: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
