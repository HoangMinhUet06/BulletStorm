
#include "Player.h"
#include <SDL_image.h>
#include <iostream>

Player::Player(int x, int y, SDL_Renderer* renderer)
    : x(x),
      y(y),
      velocityX(0),
      velocityY(0),
      renderer(renderer),
      isJumping(false),
      isMovingLeft(false),
      isAtEdge(false),
      startJumpY(y),
      initialY(y),
      lastAnimationTime(SDL_GetTicks()),
      currentFrame(0),
      health(3),
      score(0),
      isInvulnerable(false),
      lastHitTime(0) {

    // Load textures với đường dẫn đầy đủ
    idleTextureRight = loadTexture("C:/BulletStorm/BulletStormX/assets/images/_Idle.png", renderer);
    idleTexture1Right = loadTexture("C:/BulletStorm/BulletStormX/assets/images/_Idle1.png", renderer);
    runTextureRight = loadTexture("C:/BulletStorm/BulletStormX/assets/images/_Run.png", renderer);
    jumpTextureRight = loadTexture("C:/BulletStorm/BulletStormX/assets/images/_Jump.png", renderer);

    // Set initial texture
    currentTexture = idleTextureRight;
    flip = SDL_FLIP_NONE;
}


Player::~Player() {
    SDL_DestroyTexture(idleTextureRight);
    SDL_DestroyTexture(idleTexture1Right);
    SDL_DestroyTexture(runTextureRight);
    SDL_DestroyTexture(jumpTextureRight);
}

SDL_Texture* Player::loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Không thể load ảnh: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_SPACE:
                if (!isJumping) {
                    isJumping = true;
                    startJumpY = y;
                    velocityY = JUMP_VELOCITY;
                    currentTexture = jumpTextureRight;
                }
                break;
            case SDLK_LEFT:
                velocityX = -MOVE_SPEED;
                isMovingLeft = true;
                flip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_RIGHT:
                velocityX = MOVE_SPEED;
                isMovingLeft = false;
                flip = SDL_FLIP_NONE;
                break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                if (velocityX < 0) {
                    velocityX = 0;
                    isMovingLeft = false;
                }
                break;
            case SDLK_RIGHT:
                if (velocityX > 0) {
                    velocityX = 0;
                    isMovingLeft = false;
                }
                break;
        }
    }
}
void Player::update() {
    // Di chuyển theo trục X
    x += velocityX;

    // Giới hạn trong màn hình
    if (x < 0) {
        x = 0;
        velocityX = 0;
    }
    else if (x > 1024 - PLAYER_WIDTH) {
        x = 1024 - PLAYER_WIDTH;
        velocityX = 0;
    }

    // Xử lý nhảy và trọng lực
    if (isJumping) {
        // Giới hạn tốc độ rơi tối đa
        const float MAX_FALL_SPEED = 0.5f;

        velocityY += GRAVITY;
        // Giới hạn tốc độ rơi
        if (velocityY > MAX_FALL_SPEED) {
            velocityY = MAX_FALL_SPEED;
        }

        y += velocityY;

        // Kiểm tra khi chạm đất
        if (y >= initialY) {
            y = initialY;
            isJumping = false;
            velocityY = 0;
            currentTexture = idleTextureRight;
        }
    }

    updateAnimation();
}

void Player::incrementScore(int points) {
    score += points; // Tăng điểm
}

int Player::getScore() const {
    return score; // Lấy điểm hiện tại
}

void Player::updateAnimation() {
    Uint32 currentTime = SDL_GetTicks();

    // Cập nhật frame mỗi t
    if (currentTime - lastAnimationTime >= 300) {
        if (isJumping) {
            currentTexture = jumpTextureRight;
        }
        else if (velocityX != 0) {
            currentTexture = runTextureRight;

        }
        else {
            // Đứng yên, chuyển đổi giữa 2 texture idle
            if (currentTexture == idleTextureRight)
                currentTexture = idleTexture1Right;
            else
                currentTexture = idleTextureRight;
        }

        lastAnimationTime = currentTime;
    }
}

void Player::render() {
    SDL_Rect renderQuad = {
        static_cast<int>(x),
        static_cast<int>(y),
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };

    // Xử lý hiệu ứng nhấp nháy khi bị trúng đạn
    if (isInvulnerable) {
        Uint32 currentTime = SDL_GetTicks();
        // Nhấp nháy mỗi 100ms trong 500ms
        if ((currentTime - lastHitTime) % 200 < 100) {
            // Ẩn nhân vật (không vẽ gì cả)
            return;
        }
    }

    // Vẽ nhân vật
    SDL_RenderCopyEx(renderer, currentTexture, nullptr, &renderQuad, 0, nullptr, flip);
}

void Player::takeDamage() {
    if (!isInvulnerable) {
        health--;
        isInvulnerable = true;
        lastHitTime = SDL_GetTicks();
        std::cout << "Player hit! Health: " << health << std::endl;
        // Sau 1 giây, kết thúc trạng thái bất tử
        SDL_AddTimer(INVULNERABLE_TIME, [](Uint32 interval, void* param) -> Uint32 {
            Player* player = static_cast<Player*>(param);
            player->isInvulnerable = false;
            return 0;
        }, this);
    }
}

bool Player::isAlive() const {
    return health > 0;
}

int Player::getHealth() const {
    return health;
}

SDL_Rect Player::getCollisionBox() const {
    return {
        static_cast<int>(x + 16),
        static_cast<int>(y + 16),
        32,  // Thu nhỏ vùng va chạm
        32
    };
}
