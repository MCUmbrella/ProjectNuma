//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_PLAYERWEAPONS_H
#define PROJECTNUMA_PLAYERWEAPONS_H

#include "../GeneralDefinitions.h"
#include "Weapon.h"

/**
 * public class PlayerWeapon0 extends Weapon
 */
class PlayerWeapon0 : public Weapon
{
public:
    PlayerWeapon0();

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon1 extends Weapon
 */
class PlayerWeapon1 : public Weapon
{
public:
    PlayerWeapon1();

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon2 extends Weapon
 */
class PlayerWeapon2 : public Weapon
{
public:
    PlayerWeapon2();

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon3 extends Weapon
 */
class PlayerWeapon3 : public Weapon
{
public:
    PlayerWeapon3();

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_PLAYERWEAPONS_H
