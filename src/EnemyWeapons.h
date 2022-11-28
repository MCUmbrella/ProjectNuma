//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_ENEMYWEAPONS_H
#define PROJECTNUMA_ENEMYWEAPONS_H

/**
 * public class EnemyWeapon0 extends Weapon
 */
class EnemyWeapon0 : public Weapon
{
public:
    EnemyWeapon0()
    {
        bulletSpeed = 10;
        bulletDamage = 5;
        reloadTicks = 100;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon0ebullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon1 extends Weapon
 */
class EnemyWeapon1 : public Weapon
{
public:
    EnemyWeapon1()
    {
        bulletSpeed = 8;
        bulletDamage = 8;
        reloadTicks = 40;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon1ebullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon2 extends Weapon
 */
class EnemyWeapon2 : public Weapon
{
public:
    EnemyWeapon2()
    {
        bulletSpeed = 5;
        bulletDamage = 20;
        reloadTicks = 200;
        bulletTexture = RenderManager.getTexture("assets/projectnuma/textures/entity/weapon2ebullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

#endif //PROJECTNUMA_ENEMYWEAPONS_H
