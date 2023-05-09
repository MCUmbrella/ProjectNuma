//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_PLAYERWEAPON1_H
#define PROJECTNUMA_PLAYERWEAPON1_H

#include "../Weapon.h"

class PlayerWeapon1 : public Weapon
{
public:
    PlayerWeapon1();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_PLAYERWEAPON1_H
