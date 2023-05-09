//
// Created by mcumbrella on 23-5-8.
//

#include "PlayerWeapons.h"
#include "../App.h"
#include "../entity/Bullet.h"
#include "../Random.h"
#include "../RenderManager.h"
#include "../SoundManager.h"

PlayerWeapon0::PlayerWeapon0()
{
    name = "Small machine gun";
    bulletSpeed = 20;
    bulletDamage = 10;
    reloadTicks = 10;
    App& a = App::getInstance();
    RenderManager* rm = a.renderManager;
    bulletTexture = rm->getTexture("assets/projectnuma/textures/entity/weapon0bullet.png");
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
