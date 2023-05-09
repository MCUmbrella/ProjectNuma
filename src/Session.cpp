//
// Created by mcumbrella on 23-5-8.
//

#include "Session.h"

void Session::load()
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

void Session::save()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Saving session file");
    ofstream f(FILE_NAME);
    f << hp << ' ' << credit << ' ' << unlockedWeapons << ' ' << unlockedLevels << "\n "
      << killboardTotal[0] << ' ' << killboardTotal[1] << ' ' << killboardTotal[2] << ' ' << killboardTotal[3]
      << '\n' << flush;
    f.close();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Session file saved");
}
