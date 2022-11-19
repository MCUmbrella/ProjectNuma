//
// Created by mcumbrella on 22-11-19.
//

#ifndef PROJECTNUMA_LEVELUTIL_H
#define PROJECTNUMA_LEVELUTIL_H

#include <fstream>

class Level
{
public:
    int killGoal[4]{INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX};
    double spawnRateBase = 0.03;
    double spawnRateEnemy0 = 0.5;
    double spawnRateEnemy1 = 0.4;
    double spawnRateEnemy2 = 0.25;
    double spawnRateEnemy3 = 0.05;
    std::string name = "Unnamed level";
};

static class
{
public:
    Level loadLevel(const char* fileName) //TODO
    {
        return Level();
    }
} LevelUtil;

#endif //PROJECTNUMA_LEVELUTIL_H
