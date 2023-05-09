//
// Created by mcumbrella on 23-5-9.
//

#include "PlayerWeapon0.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../../entity/Bullet.h"

PlayerWeapon0::PlayerWeapon0()
{
    name = "Small machine gun";
    bulletSpeed = 20;
    bulletDamage = 10;
    reloadTicks = 10;
    bulletTexture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/weapon0bullet.png");
}

void PlayerWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    degree += App::getInstance().getRandom().nextDouble(6.0) - 3.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(0, -b->height / 2);
    b->texture = bulletTexture;
    App::getInstance().addEntity(b);
    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/item/weapon0.wav");
}
