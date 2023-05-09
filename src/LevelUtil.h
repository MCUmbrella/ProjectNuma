//
// Created by mcumbrella on 22-11-19.
//

#ifndef PROJECTNUMA_LEVELUTIL_H
#define PROJECTNUMA_LEVELUTIL_H

class Level
{
public:
    bool isPassed = false, isFailed = false;
    /**
     * Maximum tries of a level.
     * If player died this time(s), the level is failed. Must be larger than 0.
     */
    int playerLife = 1;
    /**
     * If player killed this count of a type of enemy, the level is passed.
     */
    int killGoal[4]{2147483647, 2147483647, 2147483647, 2147483647};
    /**
     * How many enemies (with specified type) can be in the screen together?
     * If the count is 0, this type of enemy will not spawn at all.
     */
    int maxEnemiesInScreen[4] = {2147483647, 2147483647, 2147483647, 2147483647};
    /**
     * Base speed of random spawning enemies.
     * Must between 0 and 1. Larger number means higher spawning speed.
     * Set to 0 to stop random spawning enemies.
     */
    double randomSpawnSpeed = 0.03;
    /**
     * The spawning speed of a specified type of enemy.
     * Must between 0 and 1. Larger number means this type of enemy is more common.
     * Set to 0 to stop random spawning this type of enemy.
     */
    double enemySpawnRate[4] = {0.5, 0.4, 0.25, 0.05};
    /**
     * Infinite levels and custom levels must keep this 0.
     */
    unsigned long id = 0;
    /**
     * The name of the level.
     */
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
              >> l.maxEnemiesInScreen[0] >> l.maxEnemiesInScreen[1] >> l.maxEnemiesInScreen[2] >> l.maxEnemiesInScreen[3]
              >> l.randomSpawnSpeed
              >> l.enemySpawnRate[0] >> l.enemySpawnRate[1] >> l.enemySpawnRate[2] >> l.enemySpawnRate[3]
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
