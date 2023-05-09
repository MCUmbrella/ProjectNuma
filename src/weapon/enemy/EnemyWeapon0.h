//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_ENEMYWEAPON0_H
#define PROJECTNUMA_ENEMYWEAPON0_H

#include "../Weapon.h"

class EnemyWeapon0 : public Weapon
{
public:
    EnemyWeapon0();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_ENEMYWEAPON0_H
