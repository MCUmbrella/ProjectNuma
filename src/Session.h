//
// Created by mcumbrella on 22-11-14.
//

#ifndef PROJECTNUMA_SESSION_H
#define PROJECTNUMA_SESSION_H

#include "GeneralDefinitions.h"

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
    void load();

    /**
     * Save player data to "session.txt".
     */
    void save();
};

#endif //PROJECTNUMA_SESSION_H
