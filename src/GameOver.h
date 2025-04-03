#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL.h>
#include <SDL_ttf.h>

class GameOver {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    int health;
    int score;
    bool isGameOver;

    // Biến thời gian
    Uint32 lastScoreTime;    // Thời điểm tính điểm cuối cùng
    Uint32 lastBonusTime;    // Thời điểm tính điểm thưởng cuối cùng
    const int SCORE_INTERVAL = 1000;    // 1 giây
    const int BONUS_INTERVAL = 10000;   // 10 giây
    const int SCORE_PER_SECOND = 10;    // Điểm mỗi giây
    const int BONUS_SCORE = 500;        // Điểm thưởng mỗi 10 giây

public:
    GameOver(SDL_Renderer* renderer, TTF_Font* font);
    ~GameOver();

    void update();
    void render();
    void takeDamage();
    void reset();
    bool isOver() const;
    int getHealth() const;
    int getScore() const;
};

#endif
