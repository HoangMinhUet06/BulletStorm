#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class Audio {
public:
    Audio();  // Constructor
    ~Audio(); // Destructor

    bool loadMusic(const std::string& path); // Load nhạc từ đường dẫn
    void playMusic();   // Phát nhạc nền
    void stopMusic();   // Dừng nhạc

private:
    Mix_Music* bgMusic; // Biến chứa nhạc nền
};

#endif
