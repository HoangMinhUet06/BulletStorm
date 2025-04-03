#ifndef BULLET_H_
#define BULLET_H_

#include <SDL.h>
#include <vector>
#include <cmath>

class Bullet {
private:
    float x, y;
    float velocityX, velocityY;
    float angle;
    float speed;
    bool isActive;
    SDL_Color color;
    SDL_Renderer* renderer;

    static const int BULLET_RADIUS = 5;
    static constexpr float PI = 3.14159f;

public:
    Bullet(float x, float y, float angle, float speed, SDL_Color color, SDL_Renderer* renderer);
    ~Bullet();

    void update();
    void render(SDL_Renderer* renderer);
    bool isOffScreen();

    bool getActive() const { return isActive; }
    void setActive(bool active) { isActive = active; }

    SDL_Rect getRect() const;
};

class BulletManager {
private:
    std::vector<Bullet*> bullets;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    float spawnY;

    enum PatternState {
        CORNER_PATTERN,      // Bắn từ 4 góc
        CENTER_PATTERN,      // Bắn từ trung tâm
        CONE_PATTERN,        // Bắn hình nón
        SIDE_PATTERN,        // Bắn từ 4 bên
        SPIRAL_PATTERN,      // Xoáy tròn
        SPREAD_PATTERN       // Tỏa ra
    };

    PatternState currentPattern;
    Uint32 lastPatternTime;
    Uint32 lastCornerTime;
    Uint32 lastBulletTime;
    SDL_Color bulletColor;

    // Điều chỉnh các hằng số
    static constexpr float BULLET_SPEED = 1.8f;
    static constexpr int MAX_BULLETS = 10000;
    static constexpr Uint32 PATTERN_DELAY = 5000;
    static constexpr Uint32 CORNER_DELAY = 2000;
    static constexpr Uint32 BULLET_DELAY = 1200;
    static constexpr float PI = 3.14159f;

public:
    BulletManager(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~BulletManager();

    void createBullet(float x, float y, float angle);
    void createCornerPattern();
    void createCenterPattern();
    void createConePattern();
    void createSidePattern();
    void createSpiralPattern();
    void createSpreadPattern();
    void updatePatterns();
    void update();
    void render(SDL_Renderer* renderer);
    void clearBullets();
    const std::vector<Bullet*>& getBullets() const { return bullets; }
    void setBulletColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
};

#endif
