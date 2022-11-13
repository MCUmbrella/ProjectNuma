//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_SOUNDUTIL_H
#define PROJECTNUMA_SOUNDUTIL_H

#include <SDL2/SDL_mixer.h>

static class
{
public:
    static Mix_Chunk* loadSound(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading sound: %s", fileName);
        Mix_Chunk* s = Mix_LoadWAV(fileName);
        if (s == null)throw std::runtime_error(std::string("Sound not found: ").append(fileName));
    }

    static void playSound(Mix_Chunk* sound)
    {
        Mix_PlayChannel(-1, sound, false);
    }

    static void stopBGM()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Stopping BGM");
        Mix_HaltMusic();
    }

    static Mix_Music* loadMusic(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading music: %s", fileName);
        Mix_Music* m = Mix_LoadMUS(fileName);
        if (m == null)throw std::runtime_error(std::string("Music not found: ").append(fileName));
    }

    static void setBGM(Mix_Music* bgm)
    {
        stopBGM();
        Mix_PlayMusic(bgm, -1);
    }

    static void init()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            printf("Couldn't initialize SDL Mixer\n");
            exit(1);
        }
        Mix_AllocateChannels(64);
    }
} SoundUtil;

#endif //PROJECTNUMA_SOUNDUTIL_H
