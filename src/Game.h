#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "Player.h"
#include "Bullet.h"

class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void loadResources();
    void handleEvent(SDL_Event& e);
    void update();
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* background;
    SDL_Texture* character;
    SDL_Rect characterRect;

    // Thêm các thành viên mới
    Player* player;
    BulletManager* bulletManager;
};

#endif
