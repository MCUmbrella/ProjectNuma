//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_PLAYER_H
#define PROJECTNUMA_PLAYER_H

#include "Entity.h"

/**
 * public class Player extends Entity
 */
class Player : public Entity
{
public:
    int life = 1, invincibleTicks = 150;
    double speedModifier = 1.0;
    vector<Weapon*> currentWeapons;

    Player();
};

#endif //PROJECTNUMA_PLAYER_H
