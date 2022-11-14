//
// Created by mcumbrella on 22-11-14.
//

#ifndef PROJECTNUMA_SESSIONUTIL_H
#define PROJECTNUMA_SESSIONUTIL_H

class Session
{
public:
    const char* FILE_NAME = "data/save.sav";
    int hp = 50;
    unsigned long long credit = 0, unlockedWeapons = 1, unlockedLevels = 1;

    void load()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading session file");
        //TODO
    }
    void save()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Saving session file");
        //TODO
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Session file saved");
    }
};

#endif //PROJECTNUMA_SESSIONUTIL_H
