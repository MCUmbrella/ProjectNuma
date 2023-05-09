//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_PLAYERWEAPON0_H
#define PROJECTNUMA_PLAYERWEAPON0_H

#include "../Weapon.h"

class PlayerWeapon0 : public Weapon
{
public:
    PlayerWeapon0();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_PLAYERWEAPON0_H
