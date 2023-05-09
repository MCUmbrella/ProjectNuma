//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_ENEMYWEAPON1_H
#define PROJECTNUMA_ENEMYWEAPON1_H

#include "../Weapon.h"

class EnemyWeapon1 : public Weapon
{
public:
    EnemyWeapon1();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_ENEMYWEAPON1_H
