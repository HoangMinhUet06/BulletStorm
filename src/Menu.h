
#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

// Danh sách các tùy chọn menu
enum MenuOption { PLAY, TUTORIAL, SETTING, NONE };

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void handleEvent(SDL_Event& e);
    void render();
    MenuOption getSelectedOption();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color normalColor = {255, 255, 255}; // Màu trắng
    SDL_Color hoverColor = {0, 0, 0};        // Màu đen
    int selectedOption = NONE; // Mục menu đang được chọn

    struct MenuItem {
        std::string text;
        SDL_Rect rect;
        SDL_Texture* textureNormal;
        SDL_Texture* textureHover;
    };

    MenuItem playButton;
    MenuItem tutorialButton;
    MenuItem settingButton;

    SDL_Texture* createTextTexture(const std::string& text, SDL_Color color);
};

#endif
