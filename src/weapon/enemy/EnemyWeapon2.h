//
// Created by mcumbrella on 23-5-9.
//

#ifndef PROJECTNUMA_ENEMYWEAPON2_H
#define PROJECTNUMA_ENEMYWEAPON2_H

#include "../Weapon.h"

class EnemyWeapon2 : public Weapon
{
public:
    EnemyWeapon2();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_ENEMYWEAPON2_H
