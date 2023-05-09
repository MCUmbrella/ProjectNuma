//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_PLAYERWEAPON2_H
#define PROJECTNUMA_PLAYERWEAPON2_H

#include "../Weapon.h"

class PlayerWeapon2 : public Weapon
{
public:
    PlayerWeapon2();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_PLAYERWEAPON2_H
