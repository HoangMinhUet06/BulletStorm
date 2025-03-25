#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Player.h"
#include "Bullet.h"  // Thêm include cho Bullet

// Hàm tải ảnh (texture) từ file
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Không thể load ảnh: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
 // Enum để quản lý trạng thái game
enum GameState {
    MENU_STATE,
    PLAYING_STATE,
    TUTORIAL_STATE,
    SETTING_STATE
};

int main(int argc, char* args[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Khởi tạo SDL_image (quan trọng để load texture)
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Khởi tạo cửa sổ
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 640;
    SDL_Window* window = SDL_CreateWindow("Bullet Storm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Khởi tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Tải nền
    SDL_Texture* background = loadTexture("C:/CodeBlock/BulletStorm/assets/images/background.png", renderer);
    // Khởi tạo player (vị trí ban đầu: 64, 640-64-20)
    Player* player = new Player(64, SCREEN_HEIGHT-64-20, renderer);

    // Khởi tạo BulletManager
    BulletManager* bulletManager = new BulletManager(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Đặt màu cho đạn (màu vàng)
    bulletManager->setBulletColor(255, 255, 0);

    // Biến điều khiển vòng lặp chính
    bool quit = false;
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 frameTime;

    // Vòng lặp game
    while (!quit) {
        frameTime = SDL_GetTicks(); // Lấy thời gian hiện tại

        // Kiểm tra sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            player->handleEvent(e);  // Xử lý sự kiện cho player
        }

        // Cập nhật vị trí và trạng thái của nhân vật
        player->update();

        // Cập nhật đạn
        bulletManager->update();

        // Kiểm tra va chạm giữa player và đạn
        SDL_Rect playerBox = player->getCollisionBox();
    const std::vector<Bullet*>& bullets = bulletManager->getBullets();

    for (const auto& bullet : bullets) {
        if (bullet->getActive()) {  // Chỉ kiểm tra đạn đang hoạt động
            SDL_Rect bulletBox = bullet->getRect();
            if (SDL_HasIntersection(&playerBox, &bulletBox)) {
                player->takeDamage();
                bullet->setActive(false);  // Hủy đạn sau khi va chạm

                if (!player->isAlive()) {
                    std::cout << "Game Over!" << std::endl;
                    quit = true;
                    break;
                }
            }
        }
    }
        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ nền
        SDL_RenderCopy(renderer, background, nullptr, nullptr);

        // Vẽ đạn (vẽ trước nhân vật để nhân vật hiển thị trên đạn)
        bulletManager->render();

        // Vẽ nhân vật
        player->render();

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);

        // Giới hạn FPS (khoảng 60 FPS)
        Uint32 frameDuration = SDL_GetTicks() - frameTime;
        if (frameDuration < 16) {
            SDL_Delay(16 - frameDuration);
        }
    }

    // Giải phóng bộ nhớ và đóng SDL
    delete player;
    delete bulletManager;
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
