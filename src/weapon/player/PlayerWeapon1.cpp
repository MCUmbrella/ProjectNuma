//
// Created by mcumbrella on 23-5-9.
//

#include "PlayerWeapon1.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../../entity/Bullet.h"

PlayerWeapon1::PlayerWeapon1()
{
    name = "Shotgun";
    bulletSpeed = 20;
    bulletDamage = 7;
    reloadTicks = 20;
    bulletTexture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/weapon1bullet.png");
}

void PlayerWeapon1::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    degree += App::getInstance().getRandom().nextDouble(6.0) - 3.0;
    for (int i = -2; i != 3; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 8, 8,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 3
        );
        b->move(0, -b->height / 2);
        b->texture = bulletTexture;
        App::getInstance().addEntity(b);
    }
    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/item/weapon1.wav");
}
