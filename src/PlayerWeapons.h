//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_PLAYERWEAPONS_H
#define PROJECTNUMA_PLAYERWEAPONS_H

/**
 * public class PlayerWeapon0 extends Weapon
 */
class PlayerWeapon0 : public Weapon
{
public:
    PlayerWeapon0()
    {
        name = "Small machine gun";
        bulletSpeed = 20;
        bulletDamage = 10;
        reloadTicks = 10;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon0bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon1 extends Weapon
 */
class PlayerWeapon1 : public Weapon
{
public:
    PlayerWeapon1()
    {
        name = "Shotgun";
        bulletSpeed = 20;
        bulletDamage = 7;
        reloadTicks = 20;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon1bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon2 extends Weapon
 */
class PlayerWeapon2 : public Weapon
{
public:
    PlayerWeapon2()
    {
        name = "Blaster";
        bulletSpeed = 20;
        bulletDamage = 10;
        reloadTicks = 100;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon2bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon3 extends Weapon
 */
class PlayerWeapon3 : public Weapon
{
public:
    PlayerWeapon3()
    {
        name = "Laser cannon";
        bulletSpeed = 50;
        bulletDamage = 100;
        reloadTicks = 60;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon3bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_PLAYERWEAPONS_H
