
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void loadResources();  // Hàm để load lại background, nhân vật, map
    void handleEvent(SDL_Event& e);
    void update();
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* background;
    SDL_Texture* character;
    SDL_Rect characterRect;
};

#endif
