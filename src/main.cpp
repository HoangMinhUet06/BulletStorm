#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include "HUD.h"
#include "Audio.h"
#include "Pause.h"
#include "GameState.h"
#include "Menu.h"
#include "GameOver.h"
#include "Game.h"

// Hàm tiện ích để tải texture từ file
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

// Hàm tiện ích để tạo texture từ văn bản
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

int main(int argc, char* args[]) {
    // Khởi tạo bộ tạo số ngẫu nhiên
    srand(static_cast<unsigned int>(time(nullptr)));

    // ===== KHỞI TẠO SDL VÀ CÁC THƯ VIỆN =====
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // ===== TẠO CỬA SỔ VÀ RENDERER =====
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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // ===== KHỞI TẠO ÂM THANH =====
    Audio audio;
    if (!audio.loadMusic("C:/BulletStorm/BulletStormX/assets/sounds/background_music.mp3")) {
        return -1;
    }
    audio.playMusic();

    // ===== LOAD TÀI NGUYÊN =====
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

    // ===== KHỞI TẠO CÁC ĐỐI TƯỢNG GAME =====
    Menu menu(renderer);
    GameOver* gameOver = new GameOver(renderer, font);
    HUD* hud = new HUD(renderer, font, gameOver);
    PauseMenu pauseMenu(renderer);

    // Tạo nút PLAY
    SDL_Color white = {255, 255, 255};
    SDL_Texture* playText = renderText("PLAY", white, font, renderer);
    SDL_Rect playRect = {SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 40, 160, 80};

    // ===== KHỞI TẠO BIẾN ĐIỀU KHIỂN =====
    GameState gameState = MENU_STATE;
    SDL_Texture* gameBackground = nullptr;
    Player* player = nullptr;
    BulletManager* bulletManager = nullptr;
    bool quit = false;
    SDL_Event e;

    // ===== VÒNG LẶP CHÍNH CỦA GAME =====
    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Xử lý sự kiện menu
            if (gameState == MENU_STATE) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    // Kiểm tra click vào nút PLAY
                    if (x >= playRect.x && x <= playRect.x + playRect.w &&
                        y >= playRect.y && y <= playRect.y + playRect.h) {
                        gameState = PLAYING_STATE;

                        // Khởi tạo game mới
                        gameBackground = loadTexture("C:/BulletStorm/BulletStormX/assets/images/background.png", renderer);
                        player = new Player(64, SCREEN_HEIGHT - 64 - 20, renderer);
                        bulletManager = new BulletManager(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                        bulletManager->setBulletColor(255, 255, 0);
                    }
                }
            }
            // Xử lý sự kiện trong game
            else if (gameState == PLAYING_STATE) {
                pauseMenu.handleEvent(e, quit, (gameState == MENU_STATE), gameState);

                if (!pauseMenu.getPaused()) {
                    gameOver->update();
                    player->handleEvent(e);
                    SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);

                    // Cập nhật trạng thái game
                    player->update();
                    bulletManager->update();

                    // Kiểm tra va chạm
                    SDL_Rect playerBox = player->getCollisionBox();
                    const std::vector<Bullet*>& bullets = bulletManager->getBullets();

                    for (Bullet* bullet : bullets) {
                        if (bullet->getActive()) {
                            SDL_Rect bulletBox = bullet->getRect();
                            if (SDL_HasIntersection(&playerBox, &bulletBox)) {
                                gameOver->takeDamage();
                                player->takeDamage();
                                bullet->setActive(false);
                            }
                        }
                    }

                    // Kiểm tra game over
                    if (gameOver->isOver()) {
    // Render màn hình game over
    gameOver->render();
    SDL_RenderPresent(renderer);

    // Đợi người chơi nhấn ENTER
    bool waitingForEnter = true;
    while (waitingForEnter) {
        SDL_Event gameOverEvent;
        while (SDL_PollEvent(&gameOverEvent)) {
            if (gameOverEvent.type == SDL_QUIT) {
                quit = true;
                waitingForEnter = false;
            }
            else if (gameOverEvent.type == SDL_KEYDOWN &&
                     gameOverEvent.key.keysym.sym == SDLK_RETURN) {
                waitingForEnter = false;
                // Reset game
                gameState = MENU_STATE;
                gameOver->reset();
                delete player;
                delete bulletManager;
                player = nullptr;
                bulletManager = nullptr;
                // Thoát khỏi vòng lặp game over
                break;
            }
        }
        // Thêm delay để giảm tải CPU
        SDL_Delay(16);
    }
    // Nếu đã nhấn ENTER, tiếp tục vòng lặp chính
    continue;
}

                    // Render game objects
                    bulletManager->render(renderer);
                    player->render();
                    hud->render();
                }
            }
        }

        // ===== RENDER MÀN HÌNH =====
        SDL_RenderClear(renderer);

        // Render menu hoặc game tùy theo trạng thái
        if (gameState == MENU_STATE) {
            if (menuBackground) SDL_RenderCopy(renderer, menuBackground, nullptr, nullptr);
            if (playText) SDL_RenderCopy(renderer, playText, nullptr, &playRect);
        }
        else if (gameState == PLAYING_STATE) {
            SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);
            player->update();
            bulletManager->update();
            bulletManager->render(renderer);
            player->render();
            hud->render();
        }

        // Render pause menu nếu đang tạm dừng
        if (pauseMenu.getPaused()) {
            pauseMenu.render();
        }

        SDL_RenderPresent(renderer);
    }

    // ===== DỌN DẸP BỘ NHỚ =====
    // Giải phóng các đối tượng game
    delete hud;
    delete gameOver;
    delete player;
    delete bulletManager;

    // Giải phóng textures
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(playText);
    SDL_DestroyTexture(gameBackground);

    // Đóng font
    TTF_CloseFont(font);

    // Giải phóng SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Đóng các thư viện SDL
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
