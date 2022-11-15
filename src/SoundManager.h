//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_SOUNDMANAGER_H
#define PROJECTNUMA_SOUNDMANAGER_H

#include <SDL2/SDL_mixer.h>

static std::map<const char*, Mix_Chunk*> sounds;
static std::map<const char*, Mix_Music*> musics;
static Mix_Music* bgm = null;

static class
{
public:
    static Mix_Chunk* loadSound(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "Loading sound: %s", fileName);
        Mix_Chunk* s = Mix_LoadWAV(fileName);
        if (s == null)throw std::runtime_error(std::string("Sound not found: ").append(fileName));
        sounds.emplace(fileName, s);
    }

    static Mix_Chunk* getSound(const char* name)
    {
        Mix_Chunk* s = sounds[name];
        if (s == null) throw std::runtime_error(std::string("Sound not found: ").append(name));
        return s;
    }

    static void playSound(Mix_Chunk* sound)
    {
        Mix_PlayChannel(-1, sound, false);
    }

    static void playSound(const char* name)
    {
        playSound(getSound(name));
    }

    static Mix_Music* loadMusic(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "Loading music: %s", fileName);
        Mix_Music* m = Mix_LoadMUS(fileName);
        if (m == null)throw std::runtime_error(std::string("Music not found: ").append(fileName));
        musics.emplace(fileName, m);
    }

    static Mix_Music* getMusic(const char* name)
    {
        Mix_Music* m = musics[name];
        if (m == null) throw std::runtime_error(std::string("Music not found: ").append(name));
        return m;
    }

    static void stopBGM()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "Stopping BGM");
        Mix_HaltMusic();
        if (bgm != null)
        {
            Mix_FreeMusic(bgm);
            bgm = null;
        }
    }

    static void setBGM(Mix_Music* m)
    {
        stopBGM();
        Mix_PlayMusic(m, -1);
        bgm = m;
    }

    static void setBGM(const char* name)
    {
        setBGM(getMusic(name));
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
} SoundManager;

#endif //PROJECTNUMA_SOUNDMANAGER_H
