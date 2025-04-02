#include "Game.h"
#include <SDL_image.h>
#include <iostream>

Game::Game(SDL_Renderer* renderer) : renderer(renderer) {
    // Khởi tạo các thành viên
    player = nullptr;
    bulletManager = nullptr;
    background = nullptr;
    character = nullptr;
}

Game::~Game() {
    // Giải phóng bộ nhớ
    delete player;
    delete bulletManager;
    if (background) SDL_DestroyTexture(background);
    if (character) SDL_DestroyTexture(character);
}

void Game::update() {
    if (player && bulletManager) {
        // Cập nhật trạng thái người chơi
        player->update();

        // Cập nhật trạng thái đạn
        bulletManager->update();

        // Kiểm tra va chạm
        SDL_Rect playerBox = player->getCollisionBox();
        const std::vector<Bullet*>& bullets = bulletManager->getBullets();

        for (Bullet* bullet : bullets) {
            if (bullet->getActive()) {
                SDL_Rect bulletBox = bullet->getRect();
                if (SDL_HasIntersection(&playerBox, &bulletBox)) {
                    player->takeDamage();
                    bullet->setActive(false);
                }
            }
        }
    }
}

void Game::render() {
    // Xóa màn hình
    SDL_RenderClear(renderer);

    // Vẽ background
    if (background) {
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
    }

    // Vẽ đạn
    if (bulletManager) {
        bulletManager->render(renderer);
    }

    // Vẽ người chơi
    if (player) {
        player->render();
    }

    // Hiển thị màn hình
    SDL_RenderPresent(renderer);
}

void Game::loadResources() {
    // Load background
    background = IMG_LoadTexture(renderer, "C:/BulletStorm/BulletStormX/assets/images/background.png");

    // Khởi tạo người chơi
    player = new Player(64, 768 - 64 - 20, renderer);

    // Khởi tạo quản lý đạn
    bulletManager = new BulletManager(renderer, 1024, 768);
    bulletManager->setBulletColor(255, 255, 0);
}

void Game::handleEvent(SDL_Event& e) {
    if (player) {
        player->handleEvent(e);
    }
}
