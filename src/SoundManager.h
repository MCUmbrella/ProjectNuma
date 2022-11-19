//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_SOUNDMANAGER_H
#define PROJECTNUMA_SOUNDMANAGER_H

static map<const char*, Mix_Chunk*> sounds;
static map<const char*, Mix_Music*> musics;
static Mix_Music* bgm = null;

static class
{
public:
    static Mix_Chunk* loadSound(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading sound: %s", fileName);
        Mix_Chunk* s = Mix_LoadWAV(fileName);
        if (s == null)throw runtime_error(string("Sound not found: ") + fileName);
        sounds.emplace(fileName, s);
    }

    static Mix_Chunk* getSound(const char* name)
    {
        Mix_Chunk* s = sounds[name];
        if (s == null) throw runtime_error(string("Sound not found: ") + name);
        return s;
    }

    static void playSound(Mix_Chunk* sound)
    {
        if (sound == null) throw runtime_error("Null pointer when passed to playSound()");
        Mix_PlayChannel(-1, sound, false);
    }

    static void playSound(const char* name)
    {
        playSound(getSound(name));
    }

    static Mix_Music* loadMusic(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading music: %s", fileName);
        Mix_Music* m = Mix_LoadMUS(fileName);
        if (m == null)throw runtime_error(string("Music not found: ") + fileName);
        musics.emplace(fileName, m);
    }

    static Mix_Music* getMusic(const char* name)
    {
        Mix_Music* m = musics[name];
        if (m == null) throw runtime_error(string("Music not found: ") + name);
        return m;
    }

    static void stopBGM()
    {
        if (bgm != null)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Stopping BGM");
            Mix_HaltMusic();
            bgm = null;
        }
    }

    static void setBGM(Mix_Music* m)
    {
        if (m == null) throw runtime_error("Null pointer passed to setBGM()");
        stopBGM();
        Mix_PlayMusic(m, -1);
        bgm = m;
    }

    static void setBGM(const char* name)
    {
        setBGM(getMusic(name));
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "BGM set to %s", name);
    }

    static void init()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            printf("Couldn't initialize SDL Mixer\n");
            exit(1);
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing render manager");
        Mix_AllocateChannels(64);
        for (const char* a: soundFiles)
            loadSound(a);
        for (const char* a: musicFiles)
            loadMusic(a);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sound manager initialization completed");
    }

    static void shutdown()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sound manager is being shut down now");
        stopBGM();
        Mix_Quit();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sound manager is down");
    }
} SoundManager;

#endif //PROJECTNUMA_SOUNDMANAGER_H
