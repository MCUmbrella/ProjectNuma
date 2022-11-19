//
// Created by mcumbrella on 22-11-19.
//

#ifndef PROJECTNUMA_LEVELUTIL_H
#define PROJECTNUMA_LEVELUTIL_H

#include <fstream>

class Level
{
public:
    bool isPassed = false, isFailed = false;
    int playerLife = 1;
    int killGoal[4]{2147483647, 2147483647, 2147483647, 2147483647};
    double spawnRateBase = 0.03;
    double spawnRateEnemy0 = 0.5;
    double spawnRateEnemy1 = 0.4;
    double spawnRateEnemy2 = 0.25;
    double spawnRateEnemy3 = 0.05;
    uint64_t id = 0;
    string name = "Unnamed level";
};

static class
{
public:
    Level loadLevel(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading level: %s", fileName);
        ifstream f(fileName);
        if (f)
        {
            Level l;
            f >> l.playerLife
              >> l.killGoal[0] >> l.killGoal[1] >> l.killGoal[2] >> l.killGoal[3]
              >> l.spawnRateBase
              >> l.spawnRateEnemy0 >> l.spawnRateEnemy1 >> l.spawnRateEnemy2 >> l.spawnRateEnemy3
              >> l.id;
            l.name = "";
            while (!f.eof())
            {
                string s;
                f >> s;
                s += ' ';
                l.name += s;
            }
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loaded level: %s(ID: %lu)", l.name.c_str(), l.id);
            return l;
        }
        else throw runtime_error(string("Failed to load level file: ") + fileName);
    }
} LevelUtil;

#endif //PROJECTNUMA_LEVELUTIL_H
