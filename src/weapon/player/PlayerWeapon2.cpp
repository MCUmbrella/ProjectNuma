//
// Created by mcumbrella on 23-5-9.
//

#include "PlayerWeapon2.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../../entity/Bullet.h"

PlayerWeapon2::PlayerWeapon2()
{
    name = "Blaster";
    bulletSpeed = 20;
    bulletDamage = 10;
    reloadTicks = 100;
    bulletTexture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/weapon2bullet.png");
}

void PlayerWeapon2::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    degree += App::getInstance().getRandom().nextDouble(10.0) - 5.0;
    for (int i = 0; i != 36; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 16, 16,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 10
        );
        b->move(-b->width / 2, -b->height / 2);
        b->texture = bulletTexture;
        App::getInstance().addEntity(b);
    }
    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/item/weapon2.wav");
}
