#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

class HUD {
private:
    int health;         // Số máu
    int score;          // Điểm số
    SDL_Renderer* renderer;
    TTF_Font* font;     // Font để hiển thị văn bản

public:
    HUD(SDL_Renderer* gRenderer, TTF_Font* gFont);
    ~HUD();

    // Cập nhật điểm và máu
    void updateHealth(int health);
    void updateScore(int score);

    // Vẽ thanh máu và điểm số
    void render();

    // Hàm hỗ trợ để vẽ văn bản
    void renderText(const std::string& text, int x, int y, SDL_Color color);
};

#endif // HUD_H
