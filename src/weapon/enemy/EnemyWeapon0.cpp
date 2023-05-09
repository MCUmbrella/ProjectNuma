//
// Created by mcumbrella on 23-5-9.
//

#include "EnemyWeapon0.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../../entity/Bullet.h"

EnemyWeapon0::EnemyWeapon0()
{
    bulletSpeed = 10;
    bulletDamage = 5;
    reloadTicks = 100;
    bulletTexture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/weapon0ebullet.png");
}

void EnemyWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy weapon 0 fire");
    degree += App::getInstance().getRandom().nextDouble(2.0) - 1.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(-b->width / 2, -b->height / 2);
    b->texture = bulletTexture;
    App::getInstance().addEntity(b);
    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/item/weapon0e.wav");
}
