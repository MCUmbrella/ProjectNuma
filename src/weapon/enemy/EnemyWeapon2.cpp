//
// Created by mcumbrella on 23-5-9.
//

#include "EnemyWeapon2.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../../entity/Bullet.h"

EnemyWeapon2::EnemyWeapon2()
{
    bulletSpeed = 5;
    bulletDamage = 20;
    reloadTicks = 200;
    bulletTexture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/weapon2ebullet.png");
}

void EnemyWeapon2::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy weapon 2 fire");
    degree += App::getInstance().getRandom().nextDouble(6.0) - 3.0;
    for (int i = -7; i != 8; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 16, 16,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 3
        );
        b->hp = 150;
        b->move(-b->width / 2, -b->height / 2);
        b->texture = bulletTexture;
        b->afterTick = [](Entity* self) {
            self->hp--;
        };
        App::getInstance().addEntity(b);
    }
    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/item/weapon2e.wav");
}
