#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
private:
    float x, y;
    float velocityX, velocityY;
    SDL_Renderer* renderer;
    SDL_Texture* idleTextureRight;    // Texture đứng yên
    SDL_Texture* idleTexture1Right;   // Texture đứng yên thứ 2
    SDL_Texture* runTextureRight;     // Texture chạy
    SDL_Texture* jumpTextureRight;    // Texture nhảy
    SDL_Texture* currentTexture;      // Texture hiện tại

    // Thêm các thuộc tính mới
    int health;                     // Máu của người chơi
    int score;                      // Điểm số của người chơi
    bool isInvulnerable;           // Trạng thái bất tử tạm thời
    Uint32 lastHitTime;            // Thời gian bị đạn trúng cuối cùng
    static const Uint32 INVULNERABLE_TIME = 300; // Thời gian bất tử (300ms = 0.3 giây)

    bool isJumping;
    bool isMovingLeft;
    bool isAtEdge;          // Kiểm tra va chạm với rìa
    float startJumpY;       // Vị trí Y ban đầu khi nhảy
    float initialY;         // Vị trí Y mặc định

    // Animation variables
    Uint32 lastAnimationTime;
    int currentFrame;
    SDL_RendererFlip flip;

    // Constants
    static const int PLAYER_WIDTH = 64;
    static const int PLAYER_HEIGHT = 64;
    static const int ANIMATION_DELAY = 200;    // t mỗi frame
    static constexpr float JUMP_HEIGHT = 110;
    static constexpr float JUMP_VELOCITY = -1.0f;
    static constexpr float GRAVITY = 0.01f;
    static constexpr float MOVE_SPEED = 0.15f;
    // Giới hạn tốc độ rơi tối đa
    const float MAX_FALL_SPEED = 1.0f;

    SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
    void updateAnimation();

public:
    Player(int x, int y, SDL_Renderer* renderer);
    ~Player();

    SDL_Rect getCollisionBox() const;

    void takeDamage();
    bool isAlive() const;
    void handleEvent(SDL_Event& e);
    void update();
    void render();
};

#endif
