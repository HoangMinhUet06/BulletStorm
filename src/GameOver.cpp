#include "GameOver.h"
#include <iostream>

GameOver::GameOver(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), health(10), score(0), isGameOver(false) {
    lastScoreTime = SDL_GetTicks();
    lastBonusTime = SDL_GetTicks();
}

GameOver::~GameOver() {}

void GameOver::update() {
    if (!isGameOver) {
        Uint32 currentTime = SDL_GetTicks();

        // Cộng điểm mỗi giây
        if (currentTime - lastScoreTime >= SCORE_INTERVAL) {
            score += SCORE_PER_SECOND;
            lastScoreTime = currentTime;
        }

        // Cộng điểm thưởng mỗi 10 giây
        if (currentTime - lastBonusTime >= BONUS_INTERVAL) {
            score += BONUS_SCORE;
            lastBonusTime = currentTime;
        }
    }
}

void GameOver::render() {
    if (isGameOver) {
        // Vẽ nền đen mờ
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, 1024, 640};
        SDL_RenderFillRect(renderer, &overlay);

        // Vẽ "GAME OVER"
        SDL_Color red = {255, 0, 0};
        SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "GAME OVER!", red);
        if (gameOverSurface) {
            SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
            SDL_FreeSurface(gameOverSurface);

            if (gameOverTexture) {
                SDL_Rect gameOverRect = {312, 220, 400, 100};
                SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
                SDL_DestroyTexture(gameOverTexture);
            }
        }

        // Vẽ điểm số
        SDL_Color white = {255, 255, 255};
        std::string scoreText = "Final Score: " + std::to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        if (scoreSurface) {
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            SDL_FreeSurface(scoreSurface);

            if (scoreTexture) {
                SDL_Rect scoreRect = {312, 370, 400, 50};
                SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
                SDL_DestroyTexture(scoreTexture);
            }
        }

        // Vẽ hướng dẫn
        SDL_Color yellow = {255, 255, 0};
        std::string instructionText = "Press ENTER to return to menu";
        SDL_Surface* instructionSurface = TTF_RenderText_Solid(font, instructionText.c_str(), yellow);
        if (instructionSurface) {
            SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
            SDL_FreeSurface(instructionSurface);

            if (instructionTexture) {
                SDL_Rect instructionRect = {262, 470, 500, 50};
                SDL_RenderCopy(renderer, instructionTexture, nullptr, &instructionRect);
                SDL_DestroyTexture(instructionTexture);
            }
        }
    }
}

void GameOver::takeDamage() {
    if (!isGameOver) {
        health--;
        if (health <= 0) {
            isGameOver = true;
        }
    }
}

void GameOver::reset() {
    health = 10;
    score = 0;
    isGameOver = false;
    lastScoreTime = SDL_GetTicks();
    lastBonusTime = SDL_GetTicks();
}

bool GameOver::isOver() const {
    return isGameOver;
}

int GameOver::getHealth() const {
    return health;
}

int GameOver::getScore() const {
    return score;
}

