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
    int killGoal[4]{INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX};
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
    Level loadLevel(const char* fileName) //TODO
    {
        return Level();
    }
} LevelUtil;

#endif //PROJECTNUMA_LEVELUTIL_H
