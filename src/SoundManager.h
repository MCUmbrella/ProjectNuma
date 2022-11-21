//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_SOUNDMANAGER_H
#define PROJECTNUMA_SOUNDMANAGER_H

static Mix_Music* bgm = null;

static class
{
private:
    map<const char*, Mix_Chunk*> sounds;
    map<const char*, Mix_Music*> musics;
public:
    /**
     * Load a sound from file.
     * @param fileName File path.
     * @return The pointer to the sound object of the loaded sound file.
     */
    Mix_Chunk* loadSound(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading sound: %s", fileName);
        Mix_Chunk* s = Mix_LoadWAV(fileName);
        if (s == null)throw runtime_error(string("Sound not found: ") + fileName);
        sounds.emplace(fileName, s);
        return s;
    }

    /**
     * Get a loaded sound from its path.
     * @param name File path.
     */
    Mix_Chunk* getSound(const char* name)
    {
        Mix_Chunk* s = sounds[name];
        if (s == null) throw runtime_error(string("Sound not found: ") + name);
        return s;
    }

    /**
     * Play a sound object.
     * @param sound The pointer to the sound object.
     */
    void playSound(Mix_Chunk* sound)
    {
        if (sound == null) throw runtime_error("Null pointer when passed to playSound()");
        Mix_PlayChannel(-1, sound, false);
    }

    /**
     * Play a loaded sound by its file path.
     * @param name File path.
     */
    void playSound(const char* name)
    {
        playSound(getSound(name));
    }

    /**
     * Load a music from file.
     * @param fileName File path.
     * @return The pointer to the music object of the loaded music file.
     */
    Mix_Music* loadMusic(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading music: %s", fileName);
        Mix_Music* m = Mix_LoadMUS(fileName);
        if (m == null)throw runtime_error(string("Music not found: ") + fileName);
        musics.emplace(fileName, m);
        return m;
    }

    /**
     * Get a loaded music from its path.
     * @param name File path.
     */
    Mix_Music* getMusic(const char* name)
    {
        Mix_Music* m = musics[name];
        if (m == null) throw runtime_error(string("Music not found: ") + name);
        return m;
    }

    /**
     * Stop the playing music.
     */
    void stopBGM()
    {
        if (bgm != null)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Stopping BGM");
            Mix_HaltMusic();
            bgm = null;
        }
    }

    /**
     * Play the music object.
     * @param m The pointer to the music.
     */
    void setBGM(Mix_Music* m)
    {
        if (m == null) throw runtime_error("Null pointer passed to setBGM()");
        stopBGM();
        Mix_PlayMusic(m, -1);
        bgm = m;
    }

    /**
     * Play a loaded music by its file path.
     * @param name File path.
     */
    void setBGM(const char* name)
    {
        setBGM(getMusic(name));
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "BGM set to %s", name);
    }

    /**
     * Initialize sound manager.
     */
    void init()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            printf("Couldn't initialize SDL Mixer\n");
            exit(1);
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing render manager");
        Mix_AllocateChannels(64);
        for (const char* a : soundFiles)
            loadSound(a);
        for (const char* a : musicFiles)
            loadMusic(a);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sound manager initialization completed");
    }

    /**
     * Shutdown sound manager.
     */
    void shutdown()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sound manager is being shut down now");
        stopBGM();
        Mix_Quit();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sound manager is down");
    }
} SoundManager;

#endif //PROJECTNUMA_SOUNDMANAGER_H
