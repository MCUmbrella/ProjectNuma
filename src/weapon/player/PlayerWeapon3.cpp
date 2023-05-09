//
// Created by mcumbrella on 23-5-9.
//

#include "PlayerWeapon3.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../../entity/Bullet.h"

PlayerWeapon3::PlayerWeapon3()
{
    name = "Laser cannon";
    bulletSpeed = 50;
    bulletDamage = 100;
    reloadTicks = 60;
    bulletTexture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/weapon3bullet.png");
}

void PlayerWeapon3::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    Bullet* b = new Bullet(
            owner, bulletDamage, 200, 20,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(0, -b->height / 2);
    b->texture = bulletTexture;
    App::getInstance().addEntity(b);
    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/item/weapon3.wav");
}
