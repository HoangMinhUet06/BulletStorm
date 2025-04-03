#include "Bullet.h"
#include <iostream>

Bullet::Bullet(float x, float y, float angle, float speed, SDL_Color color, SDL_Renderer* renderer)
    : x(x), y(y), angle(angle), speed(speed), color(color), renderer(renderer), isActive(true) {
    velocityX = speed * 0.08f * cos(angle);
    velocityY = speed * 0.08f * sin(angle);
}

Bullet::~Bullet() {}

void Bullet::update() {
    x += velocityX;
    y += velocityY;

    if (isOffScreen()) {
        isActive = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!isActive) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int centerX = static_cast<int>(x);
    int centerY = static_cast<int>(y);

    for (int y = -BULLET_RADIUS; y <= BULLET_RADIUS; y++) {
        int width = static_cast<int>(sqrt(BULLET_RADIUS * BULLET_RADIUS - y * y));
        SDL_RenderDrawLine(renderer, centerX - width, centerY + y, centerX + width, centerY + y);
    }
}

bool Bullet::isOffScreen() {
    return (x < -BULLET_RADIUS || x > 1024 + BULLET_RADIUS ||
            y < -BULLET_RADIUS || y > 768 + BULLET_RADIUS);
}

SDL_Rect Bullet::getRect() const {
    return {static_cast<int>(x - BULLET_RADIUS), static_cast<int>(y - BULLET_RADIUS),
           BULLET_RADIUS * 2, BULLET_RADIUS * 2};
}

BulletManager::BulletManager(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
    spawnY = screenHeight * 0.75f;
    currentPattern = CORNER_PATTERN;
    lastPatternTime = SDL_GetTicks();
    lastCornerTime = SDL_GetTicks();
    lastBulletTime = SDL_GetTicks();
    bulletColor = {255, 255, 255, 255};
}

BulletManager::~BulletManager() {
    clearBullets();
}

void BulletManager::createBullet(float x, float y, float angle) {
    if (bullets.size() >= MAX_BULLETS) return;
    bullets.push_back(new Bullet(x, y, angle, BULLET_SPEED, bulletColor, renderer));
}

void BulletManager::createCornerPattern() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBulletTime < BULLET_DELAY) return;

    // Bắn từ 4 góc
    createBullet(0, 0, PI/4);                    // Góc trên trái
    createBullet(screenWidth, 0, 3*PI/4);        // Góc trên phải
    createBullet(screenWidth, screenHeight, 5*PI/4); // Góc dưới phải
    createBullet(0, screenHeight, 7*PI/4);       // Góc dưới trái

    lastBulletTime = currentTime;
}

void BulletManager::createCenterPattern() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBulletTime < BULLET_DELAY) return;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 4.0f;  // Vị trí 1/4 màn hình từ trên xuống

    // Bắn nhiều hướng từ trung tâm
    for (int i = 0; i < 8; i++) {
        float angle = (2 * PI * i) / 8;
        createBullet(centerX, centerY, angle);
    }

    lastBulletTime = currentTime;
}

void BulletManager::createConePattern() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBulletTime < BULLET_DELAY) return;

    static float coneAngle = 0;
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 4.0f;

    // Tạo pattern nón xoay
    for (int i = 0; i < 12; i++) {
        float angle = coneAngle + (PI/3 * i) / 12;  // Góc spread 60 độ
        createBullet(centerX, centerY, angle);
    }

    coneAngle += 0.1f;  // Tốc độ xoay
    lastBulletTime = currentTime;
}

void BulletManager::createSidePattern() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBulletTime < BULLET_DELAY) return;

    // Bắn từ 4 bên vuông góc
    for (int i = 0; i < 5; i++) {
        float y = screenHeight * (i + 1) / 6;  // Chia đều theo chiều dọc
        createBullet(0, y, 0);                 // Bên trái
        createBullet(screenWidth, y, PI);      // Bên phải
    }

    for (int i = 0; i < 5; i++) {
                float x = screenWidth * (i + 1) / 6;   // Chia đều theo chiều ngang
        createBullet(x, 0, PI/2);              // Bên trên
        createBullet(x, screenHeight, -PI/2);  // Bên dưới
    }

    lastBulletTime = currentTime;
}

void BulletManager::createSpiralPattern() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBulletTime < BULLET_DELAY) return;

    static float spiralAngle = 0;
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 4.0f;

    // Tạo pattern xoáy
    for (int i = 0; i < 4; i++) {
        float angle = spiralAngle + (2 * PI * i) / 4;
        createBullet(centerX, centerY, angle);
    }

    spiralAngle += 0.2f;  // Tốc độ xoay
    lastBulletTime = currentTime;
}

void BulletManager::createSpreadPattern() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastBulletTime < BULLET_DELAY) return;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 4.0f;
    int numBullets = 16;  // Số lượng đạn tỏa ra

    // Tạo pattern tỏa ra
    for (int i = 0; i < numBullets; i++) {
        float angle = (2 * PI * i) / numBullets;
        createBullet(centerX, centerY, angle);
    }

    lastBulletTime = currentTime;
}

void BulletManager::updatePatterns() {
    Uint32 currentTime = SDL_GetTicks();

    // Chuyển pattern ngẫu nhiên
    if (currentTime - lastPatternTime >= PATTERN_DELAY) {
        // Tạo số ngẫu nhiên từ 0 đến 5
        int randomPattern = rand() % 6;
        currentPattern = static_cast<PatternState>(randomPattern);
        lastPatternTime = currentTime;
    }

    // Thực hiện pattern hiện tại
    switch (currentPattern) {
        case CORNER_PATTERN:
            createCornerPattern();
            break;
        case CENTER_PATTERN:
            createCenterPattern();
            break;
        case CONE_PATTERN:
            createConePattern();
            break;
        case SIDE_PATTERN:
            createSidePattern();
            break;
        case SPIRAL_PATTERN:
            createSpiralPattern();
            break;
        case SPREAD_PATTERN:
            createSpreadPattern();
            break;
    }
}
void BulletManager::update() {
    // Cập nhật các pattern
    updatePatterns();

    // Cập nhật vị trí từng viên đạn
    for (auto it = bullets.begin(); it != bullets.end();) {
        if ((*it)->getActive()) {
            (*it)->update();
            ++it;
        } else {
            delete *it;
            it = bullets.erase(it);
        }
    }
}

void BulletManager::render(SDL_Renderer* renderer) {
    // Vẽ tất cả các đạn đang hoạt động
    for (auto bullet : bullets) {
        bullet->render(renderer);
    }
}

void BulletManager::clearBullets() {
    // Giải phóng bộ nhớ của tất cả đạn
    for (auto bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}

void BulletManager::setBulletColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    bulletColor = {r, g, b, a};
}
