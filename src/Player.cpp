
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
      health(10),
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
                    lastAnimationTime = SDL_GetTicks();
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
    // Cập nhật trạng thái bất tử
    if (isInvulnerable) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastHitTime >= INVULNERABLE_TIME) {
            isInvulnerable = false;  // Tắt trạng thái bất tử
        }
    }
    // Xử lý nhảy và trọng lực
    if (isJumping) {

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

void Player::updateAnimation() {
    Uint32 currentTime = SDL_GetTicks();

    // Cập nhật frame mỗi t
    if (currentTime - lastAnimationTime >= 300) {
        if (isJumping) {
            currentTexture = jumpTextureRight;
             lastAnimationTime = SDL_GetTicks();
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

     // Xử lý nhấp nháy
    if (isInvulnerable) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 timeSinceHit = currentTime - lastHitTime;

        // Chỉ nhấp nháy trong khoảng thời gian bất tử
        if (timeSinceHit < INVULNERABLE_TIME) {
            // Nhấp nháy mỗi 100ms
            if ((timeSinceHit / 100) % 2 == 0) {
                return;  // Ẩn nhân vật
            }
        } else {
            isInvulnerable = false;  // Đảm bảo tắt trạng thái bất tử
        }
    }

    // Vẽ nhân vật
    SDL_RenderCopyEx(renderer, currentTexture, nullptr, &renderQuad, 0, nullptr, flip);
}


bool Player::isAlive() const {
    return true;
}

void Player::takeDamage() {
    if (!isInvulnerable) {
        health -= 1;
        isInvulnerable = true;     // Kích hoạt trạng thái bất tử
        lastHitTime = SDL_GetTicks();  // Cập nhật thời điểm bị trúng
    }
}
SDL_Rect Player::getCollisionBox() const {
    return {
        static_cast<int>(x + 16),
        static_cast<int>(y + 16),
        32,  // Thu nhỏ vùng va chạm
        32
    };
}
