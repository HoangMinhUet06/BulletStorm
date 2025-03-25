#ifndef BULLET_H_
#define BULLET_H_

#include <SDL.h>
#include <vector>
#include <cmath>

// Class đại diện cho một viên đạn đơn
class Bullet {
private:
    float x, y;                 // Vị trí đạn
    float velocityX, velocityY; // Vận tốc theo hai trục
    float angle;                // Góc bắn (radian)
    float speed;                // Tốc độ đạn
    bool isActive;              // Kiểm tra đạn còn hoạt động không
    SDL_Color color;            // Màu sắc của đạn
    SDL_Renderer* renderer;     // Renderer để vẽ đạn

    // Constants
    static const int BULLET_RADIUS = 5;     // Bán kính đạn (5 pixel)
    static constexpr float PI = 3.14159f;   // Hằng số Pi

public:
    // Constructor và Destructor
    Bullet(float x, float y, float angle, float speed, SDL_Color color, SDL_Renderer* renderer);
    ~Bullet();

    // Các phương thức cơ bản
    void update();              // Cập nhật vị trí đạn
    void render();              // Vẽ đạn lên màn hình
    bool isOffScreen();         // Kiểm tra đạn nằm ngoài màn hình

    // Getters và Setters
    bool getActive() const { return isActive; }
    void setActive(bool active) { isActive = active; }

    // Hàm lấy vị trí để kiểm tra va chạm
    SDL_Rect getRect() const;
};

// Class quản lý toàn bộ đạn trong game
class BulletManager {
private:
    std::vector<Bullet*> bullets;     // Danh sách các đạn đang hoạt động
    SDL_Renderer* renderer;           // Renderer để vẽ

    int screenWidth;                  // Chiều rộng màn hình
    int screenHeight;                 // Chiều cao màn hình
    float spawnY;                     // Vị trí Y để bắn đạn (3/4 màn hình)

    // Enum quản lý trạng thái pattern ở giữa màn hình
    enum PatternState {
        STRAIGHT_PATTERN,             // Bắn thẳng
        SPIRAL_PATTERN,               // Xoáy tròn
        CONE_PATTERN                  // Hình chóp
    };

    PatternState currentPattern;      // Pattern hiện tại

    // Biến quản lý thời gian
    Uint32 lastPatternTime;           // Thời gian pattern cuối
    Uint32 lastCornerTime;            // Thời gian bắn góc cuối

    // Màu sắc đạn
    SDL_Color bulletColor;

    // Constants
    static constexpr float BULLET_SPEED = 5.0f;      // Tốc độ đạn
    static constexpr int MAX_BULLETS = 1000;         // Số đạn tối đa
    static constexpr Uint32 PATTERN_DELAY = 3000;    // 3 giây giữa các pattern
    static constexpr Uint32 CORNER_DELAY = 500;      // 0.5 giây giữa các lần bắn góc
    static constexpr float PI = 3.14159f;            // Hằng số Pi

public:
    // Constructor và Destructor
    BulletManager(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~BulletManager();

    // Các phương thức tạo đạn
    void createBullet(float x, float y, float angle);  // Tạo một viên đạn

    // Các pattern đạn
    void createStraightPattern();    // Bắn thẳng từ giữa màn hình
    void createSpreadPattern();      // Tỏa ra nhiều hướng
    void createSpiralPattern();      // Xoáy tròn
    void createConePattern();        // Hình chóp
    void shootFromCorners();         // Bắn từ 4 góc cùng lúc

    // Quản lý pattern
    void updatePatterns();           // Cập nhật các pattern theo thời gian

    // Các phương thức cơ bản
    void update();                   // Cập nhật tất cả đạn
    void render();                   // Vẽ tất cả đạn
    void clearBullets();             // Xóa tất cả đạn

    // Lấy danh sách đạn để kiểm tra va chạm
    const std::vector<Bullet*>& getBullets() const { return bullets; }

    // Đặt màu cho đạn
    void setBulletColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
};

#endif
