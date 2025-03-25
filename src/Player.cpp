
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
      flip(SDL_FLIP_NONE),
      health(3),                    // Khởi tạo với 3 máu
      isInvulnerable(false),       // Ban đầu không bất tử
      lastHitTime(0) {             // Thời gian hit cuối = 0

    // Load các texture
    idleTextureRight = loadTexture("C:/CodeBlock/BulletStorm/assets/images/_Idle.png", renderer);
    idleTexture1Right = loadTexture("C:/CodeBlock/BulletStorm/assets/images/_Idle1.png", renderer);
    runTextureRight = loadTexture("C:/CodeBlock/BulletStorm/assets/images/_Run.png", renderer);
    jumpTextureRight = loadTexture("C:/CodeBlock/BulletStorm/assets/images/_Jump.png", renderer);

    currentTexture = idleTextureRight;
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
                    velocityY = -12.0f;  // Vận tốc nhảy ban đầu
                }
                break;
            case SDLK_LEFT:
                velocityX = -5.0f;
                isMovingLeft = true;
                flip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_RIGHT:
                velocityX = 5.0f;
                isMovingLeft = false;
                flip = SDL_FLIP_NONE;
                break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                if (velocityX < 0)
                    velocityX = 0;
                break;
            case SDLK_RIGHT:
                if (velocityX > 0)
                    velocityX = 0;
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
        isAtEdge = true;
    }
    else if (x > 1024 - 64) {  // 1024 là chiều rộng màn hình, 64 là kích thước nhân vật
        x = 1024 - 64;
        isAtEdge = true;
    }
    else {
        isAtEdge = false;
    }

    // Xử lý nhảy và trọng lực
    if (isJumping) {
        velocityY += 0.5f;  // Gia tốc trọng trường
        y += velocityY;

        // Kiểm tra khi chạm đất
        if (y >= initialY) {
            y = initialY;
            isJumping = false;
            velocityY = 0;
        }
    }

    // Cập nhật animation
    updateAnimation();

    // Cập nhật trạng thái bất tử
    if (isInvulnerable) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastHitTime >= INVULNERABLE_TIME) {
            isInvulnerable = false;
        }
    }
}

void Player::updateAnimation() {
    Uint32 currentTime = SDL_GetTicks();

    // Cập nhật frame mỗi 100ms
    if (currentTime - lastAnimationTime >= 100) {
        if (isJumping) {
            currentTexture = jumpTextureRight;
            currentFrame = 0;  // Không cần animation khi nhảy
        }
        else if (velocityX != 0) {
            currentTexture = runTextureRight;
            currentFrame = (currentFrame + 1) % 8;  // 8 frames cho animation chạy
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
    // Chỉ vẽ khi không bất tử hoặc đang trong chu kỳ hiển thị của hiệu ứng nhấp nháy
    if (!isInvulnerable || (SDL_GetTicks() / 100) % 2 == 0) {
        SDL_Rect renderQuad = {
            static_cast<int>(x),
            static_cast<int>(y),
            64,  // PLAYER_WIDTH
            64   // PLAYER_HEIGHT
        };
        SDL_RenderCopyEx(renderer, currentTexture, nullptr, &renderQuad, 0, nullptr, flip);
    }
}

// Các phương thức xử lý va chạm và máu
void Player::takeDamage() {
    if (!isInvulnerable) {
        health--;
        isInvulnerable = true;
        lastHitTime = SDL_GetTicks();
        std::cout << "Player hit! Health: " << health << std::endl;
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
