#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include "HUD.h"

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

// Hàm tạo texture từ văn bản
SDL_Texture* renderText(const char* text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) {
        std::cerr << "Không thể tạo text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}

// Enum để quản lý trạng thái game
enum GameState {
    MENU_STATE,
    PLAYING_STATE
};

int main(int argc, char* args[]) {
    // Khởi tạo bộ tạo số ngẫu nhiên
    srand(static_cast<unsigned int>(time(nullptr)));
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Tạo cửa sổ
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 640;
    SDL_Window* window = SDL_CreateWindow("Bullet Storm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Load background menu
    SDL_Texture* menuBackground = loadTexture("C:/BulletStorm/BulletStormX/assets/images/start_background.jpg", renderer);
    if (!menuBackground) {
        std::cerr << "Không thể load menu background!" << std::endl;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("C:/BulletStorm/BulletStormX/assets/font/pixel_font.ttf", 48);
    if (!font) {
        std::cerr << "Không thể load font: " << TTF_GetError() << std::endl;
    }
    HUD* hud = new HUD(renderer, font);
    // Tạo texture chữ "PLAY"
    SDL_Color white = {255, 255, 255};
    SDL_Texture* playText = renderText("PLAY", white, font, renderer);
    SDL_Rect playRect = {SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 40, 160, 80}; // Đặt giữa màn hình

    // Biến điều khiển trạng thái
    GameState gameState = MENU_STATE;
    SDL_Texture* gameBackground = nullptr;
    Player* player = nullptr;
    BulletManager* bulletManager = nullptr;
    bool quit = false;
    SDL_Event e;

    // Vòng lặp game
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (gameState == MENU_STATE) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    // Kiểm tra xem chuột có nhấn vào chữ "PLAY" không
                    if (x >= playRect.x && x <= playRect.x + playRect.w &&
                        y >= playRect.y && y <= playRect.y + playRect.h) {
                        gameState = PLAYING_STATE;

                        // Load game background
                        gameBackground = loadTexture("C:/BulletStorm/BulletStormX/assets/images/background.png", renderer);

                        // Khởi tạo Player
                        player = new Player(64, SCREEN_HEIGHT - 64 - 20, renderer);

                        // Khởi tạo BulletManager
                        bulletManager = new BulletManager(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                        bulletManager->setBulletColor(255, 255, 0); // Đặt màu đạn vàng
                    }
                }
            }
             else if (gameState == PLAYING_STATE) {
                    player->handleEvent(e);
                SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);

                // Cập nhật trạng thái
                hud->updateHealth(player->getHealth());
                hud->updateScore(player->getScore());
                player->update();
                bulletManager->update();

                // Kiểm tra va chạm giữa người chơi và đạn
                SDL_Rect playerBox = player->getCollisionBox();
                const std::vector<Bullet*>& bullets = bulletManager->getBullets();

                for (Bullet* bullet : bullets) {
                    if (bullet->getActive()) {
                        SDL_Rect bulletBox = bullet->getRect();
                        if (SDL_HasIntersection(&playerBox, &bulletBox)) {
                        player->takeDamage();
                        bullet->setActive(false);
                        std::cout << "Player hit! Health: " << player->getHealth() << std::endl;
                                    }
                                }
                            }

                    // Render
                    bulletManager->render(renderer);
                    player->render();
                            }
                        }

        // Vẽ màn hình
        SDL_RenderClear(renderer);
        if (gameState == MENU_STATE) {
            if (menuBackground) SDL_RenderCopy(renderer, menuBackground, nullptr, nullptr);
            if (playText) SDL_RenderCopy(renderer, playText, nullptr, &playRect);
        } else if (gameState == PLAYING_STATE) {
            SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);
            player->update();
            bulletManager->update();
            bulletManager->render(renderer);
            player->render();
            hud->render();
        }
        SDL_RenderPresent(renderer);
    }

    // Dọn dẹp bộ nhớ
    delete hud;
    delete player;
    delete bulletManager;
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(playText);
    SDL_DestroyTexture(gameBackground);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
