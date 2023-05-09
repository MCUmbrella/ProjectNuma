//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_PLAYERWEAPON3_H
#define PROJECTNUMA_PLAYERWEAPON3_H

#include "../Weapon.h"

class PlayerWeapon3 : public Weapon
{
public:
    PlayerWeapon3();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_PLAYERWEAPON3_H
