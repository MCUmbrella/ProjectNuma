//
// Created by mcumbrella on 22-11-14.
//

#ifndef PROJECTNUMA_SESSION_H
#define PROJECTNUMA_SESSION_H

#include <fstream>

class Session
{
public:
    const char* FILE_NAME = "session.dat";
    int hp = 50;
    int killboardTotal[4]{};
    unsigned long credit = 0, unlockedWeapons = 1, unlockedLevels = 0;

    /**
     * Load player date from "session.dat".
     */
    void load()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading session file");
        ifstream f(FILE_NAME);
        if (f)
        {
            f >> hp >> credit >> unlockedWeapons >> unlockedLevels
              >> killboardTotal[0] >> killboardTotal[1] >> killboardTotal[2] >> killboardTotal[3];
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                        "Session file loaded\n\thp: %d\n\tcredit: %lu\n\tunlockedWeapons: %lu\n\tunlockedLevels: %lu",
                        hp, credit, unlockedWeapons, unlockedLevels);
        }
        else SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Session file not found or failed to open");
    }

    /**
     * Save player data to "session.txt".
     */
    void save()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Saving session file");
        ofstream f(FILE_NAME);
        f << hp << ' ' << credit << ' ' << unlockedWeapons << ' ' << unlockedLevels << "\n "
          << killboardTotal[0] << ' ' << killboardTotal[1] << ' ' << killboardTotal[2] << ' ' << killboardTotal[3]
          << '\n' << flush;
        f.close();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Session file saved");
    }
};

#endif //PROJECTNUMA_SESSION_H
