#include "Bullet.h"
#include <iostream>

//--------------------------------//
//       Lớp Bullet (đạn)         //
//--------------------------------//

Bullet::Bullet(float x, float y, float angle, float speed, SDL_Color color, SDL_Renderer* renderer)
    : x(x), y(y), angle(angle), speed(speed), color(color), renderer(renderer), isActive(true) {
    // Tính toán vận tốc theo góc bắn
    velocityX = speed * cos(angle);
    velocityY = speed * sin(angle);
}

Bullet::~Bullet() {
    // Không làm gì đặc biệt
}

void Bullet::update() {
    // Cập nhật vị trí đạn theo vận tốc
    x += velocityX;
    y += velocityY;

    // Kiểm tra đạn ra khỏi màn hình
    if (isOffScreen()) {
        isActive = false;  // Đánh dấu đạn không còn hoạt động
    }
}

// Vẽ hình tròn sử dụng thuật toán Midpoint Circle
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void Bullet::render() {
    if (!isActive) return;

    // Đặt màu cho đạn
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Vẽ đạn hình tròn đặc
    int centerX = static_cast<int>(x);
    int centerY = static_cast<int>(y);

    // Vẽ hình tròn đặc bằng cách điền các đường ngang
    for (int y = -BULLET_RADIUS; y <= BULLET_RADIUS; y++) {
        // Tính độ rộng x tại độ cao y
        int width = static_cast<int>(sqrt(BULLET_RADIUS * BULLET_RADIUS - y * y));

        // Vẽ đường ngang tại y
        SDL_RenderDrawLine(renderer, centerX - width, centerY + y, centerX + width, centerY + y);
    }
}

bool Bullet::isOffScreen() {
    // Kiểm tra đạn có nằm ngoài màn hình không
    return (x < -BULLET_RADIUS || x > 1024 + BULLET_RADIUS ||
            y < -BULLET_RADIUS || y > 768 + BULLET_RADIUS);
}

SDL_Rect Bullet::getRect() const {
    // Trả về hình chữ nhật của đạn để kiểm tra va chạm
    return {static_cast<int>(x - BULLET_RADIUS), static_cast<int>(y - BULLET_RADIUS),
           BULLET_RADIUS * 2, BULLET_RADIUS * 2};
}

//--------------------------------//
//     Lớp BulletManager         //
//--------------------------------//

BulletManager::BulletManager(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
    // Vị trí bắn đạn ở 3/4 chiều cao màn hình
    spawnY = screenHeight * 0.75f;

    // Khởi tạo pattern đầu tiên là bắn thẳng
    currentPattern = STRAIGHT_PATTERN;

    // Khởi tạo thời gian
    lastPatternTime = SDL_GetTicks();
    lastCornerTime = SDL_GetTicks();

    // Màu đạn mặc định (trắng)
    bulletColor = {255, 255, 255, 255};
}

BulletManager::~BulletManager() {
    // Giải phóng tất cả đạn
    clearBullets();
}

void BulletManager::setBulletColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    bulletColor = {r, g, b, a};
}

void BulletManager::createBullet(float x, float y, float angle) {
    // Kiểm tra giới hạn số lượng đạn
    if (bullets.size() < MAX_BULLETS) {
        bullets.push_back(new Bullet(x, y, angle, BULLET_SPEED, bulletColor, renderer));
    }
}

//--------------------------------//
//     Các pattern đạn            //
//--------------------------------//

void BulletManager::createStraightPattern() {
    // Bắn thẳng từ giữa màn hình lên trên
    float centerX = screenWidth / 2.0f;
    createBullet(centerX, spawnY, -PI/2);  // Góc -90 độ (lên trên)
}

void BulletManager::createSpreadPattern() {
    // Tạo pattern tỏa tròn từ giữa màn hình
    float centerX = screenWidth / 2.0f;
    int numBullets = 20;  // Số đạn trong một vòng tròn

    // Tạo đạn theo hình tròn
    for (int i = 0; i < numBullets; i++) {
        float angle = (2 * PI * i) / numBullets;
        createBullet(centerX, spawnY, angle);
    }
}

void BulletManager::createSpiralPattern() {
    // Tạo pattern xoáy từ giữa màn hình
    static float spiralAngle = 0;  // Lưu góc xoáy giữa các lần gọi
    float centerX = screenWidth / 2.0f;

    // Tạo 5 đạn trong một spiral
    for (int i = 0; i < 5; i++) {
        float angle = spiralAngle + (2 * PI * i) / 5;
        createBullet(centerX, spawnY, angle);
    }

    // Tăng góc xoáy cho lần bắn tiếp theo
    spiralAngle += 0.2f;
}

void BulletManager::createConePattern() {
    // Tạo pattern hình chóp/quạt từ giữa màn hình
    float centerX = screenWidth / 2.0f;
    int numBullets = 7;  // Số đạn trong một cone
    float spreadAngle = PI/3;  // Góc spread của cone (60 độ)

    // Tạo đạn trong phạm vi góc
    for (int i = 0; i < numBullets; i++) {
        // Tính góc từ -spreadAngle/2 đến +spreadAngle/2 quanh trục thẳng lên trên (-PI/2)
        float angle = -PI/2 - spreadAngle/2 + (spreadAngle * i)/(numBullets-1);
        createBullet(centerX, spawnY, angle);
    }
}

void BulletManager::shootFromCorners() {
    // Kiểm tra thời gian delay giữa các lần bắn góc
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastCornerTime < CORNER_DELAY) return;

    // Bắn từ 4 góc cùng lúc

    // Góc trên trái -> bắn hướng xuống phải
    createBullet(0, 0, PI/4);

    // Góc trên phải -> bắn hướng xuống trái
    createBullet(screenWidth, 0, 3*PI/4);

    // Góc dưới phải -> bắn hướng lên trái
    createBullet(screenWidth, screenHeight, 5*PI/4);

    // Góc dưới trái -> bắn hướng lên phải
    createBullet(0, screenHeight, 7*PI/4);

    // Cập nhật thời gian bắn cuối
    lastCornerTime = currentTime;
}

void BulletManager::updatePatterns() {
    Uint32 currentTime = SDL_GetTicks();

    // Luôn bắn từ 4 góc theo chu kỳ
    shootFromCorners();

    // Kiểm tra thời gian để chuyển pattern ở giữa màn hình
    if (currentTime - lastPatternTime >= PATTERN_DELAY) {
        // Chọn pattern tiếp theo theo thứ tự
        switch (currentPattern) {
            case STRAIGHT_PATTERN:
                // Bắn thẳng -> chuyển sang pattern xoáy
                createStraightPattern();
                currentPattern = SPIRAL_PATTERN;
                break;

            case SPIRAL_PATTERN:
                // Xoáy -> chuyển sang pattern chóp
                createSpiralPattern();
                currentPattern = CONE_PATTERN;
                break;

            case CONE_PATTERN:
                // Chóp -> quay lại pattern bắn thẳng
                createConePattern();
                currentPattern = STRAIGHT_PATTERN;
                break;
        }

        // Cập nhật thời gian pattern cuối
        lastPatternTime = currentTime;
    }
}

void BulletManager::update() {
    // Cập nhật các pattern theo thời gian
    updatePatterns();

    // Cập nhật vị trí từng viên đạn
    for (auto it = bullets.begin(); it != bullets.end();) {
        if ((*it)->getActive()) {
            (*it)->update();
            ++it;
        } else {
            // Xóa đạn không còn hoạt động
            delete *it;
            it = bullets.erase(it);
        }
    }
}

void BulletManager::render() {
    // Vẽ tất cả các đạn đang hoạt động
    for (auto bullet : bullets) {
        bullet->render();
    }
}

void BulletManager::clearBullets() {
    // Giải phóng bộ nhớ của tất cả đạn
    for (auto bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}
