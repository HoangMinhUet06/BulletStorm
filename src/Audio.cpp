#include "audio.h"
#include <iostream>

Audio::Audio() : bgMusic(nullptr) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Audio không thể khởi tạo! Lỗi: " << SDL_GetError() << std::endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer không thể mở! Lỗi: " << Mix_GetError() << std::endl;
    }
}

Audio::~Audio() {
    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
}

bool Audio::loadMusic(const std::string& path) {
    bgMusic = Mix_LoadMUS(path.c_str());
    if (!bgMusic) {
        std::cerr << "Không thể tải nhạc: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void Audio::playMusic() {
    if (bgMusic) {
        Mix_PlayMusic(bgMusic, -1); // Phát nhạc lặp vô hạn (-1)
    }
}

void Audio::stopMusic() {
    Mix_HaltMusic();
}
