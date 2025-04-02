#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

enum MenuOption { PLAY, NONE };

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
    SDL_Color normalColor = {255, 255, 255};
    SDL_Color hoverColor = {0, 0, 0};
    int selectedOption = NONE;

    struct MenuItem {
        std::string text;
        SDL_Rect rect;
        SDL_Texture* textureNormal;
        SDL_Texture* textureHover;
    };

    MenuItem playButton;

    SDL_Texture* createTextTexture(const std::string& text, SDL_Color color);
};

#endif
