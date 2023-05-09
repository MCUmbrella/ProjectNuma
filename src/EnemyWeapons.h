//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_ENEMYWEAPONS_H
#define PROJECTNUMA_ENEMYWEAPONS_H

#include "Weapon.h"

/**
 * public class EnemyWeapon0 extends Weapon
 */
class EnemyWeapon0 : public Weapon
{
public:
    EnemyWeapon0();

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon1 extends Weapon
 */
class EnemyWeapon1 : public Weapon
{
public:
    EnemyWeapon1();

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon2 extends Weapon
 */
class EnemyWeapon2 : public Weapon
{
public:
    EnemyWeapon2();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_ENEMYWEAPONS_H
