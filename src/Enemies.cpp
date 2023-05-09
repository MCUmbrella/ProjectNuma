//
// Created by mcumbrella on 23-5-8.
//

#include "Enemies.h"
#include "App.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "Random.h"
#include "CommonUtil.h"
#include "Player.h"
#include "Weapon.h"
#include "Session.h"

Enemy0::Enemy0()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 5;
    reloadTicks = 10;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 2;
    dx = -speed + App::getInstance().getRandom().nextDouble(2.0) - 1.0;
    dy = App::getInstance().getRandom().nextDouble(1.0) - 0.5;
    width = 32;
    height = 24;
    texture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/enemy0.png");
    weapon = App::getInstance().getWeapon("EnemyWeapon0");
    name = "Turret";
    onSpawn = [&](Entity* self) {
        App::getInstance().enemyCounter[0]++;
    };
    afterTick = [&](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            double deg = CommonUtil.getDegreeBetween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    App::getInstance().getPlayer()->x + App::getInstance().getPlayer()->width / 2,
                    App::getInstance().getPlayer()->y + App::getInstance().getPlayer()->height / 2
            );
            self->weapon->fire(self, deg);
            self->reloadTicks = self->weapon->reloadTicks;
        }
    };
    onDeath = [&](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
        {
            App::getInstance().soundManager->playSound("assets/projectnuma/sounds/entity/hit.wav");
            App::getInstance().killboardLevel[0]++;
        }
        App::getInstance().enemyCounter[0]--;
    };
}

Enemy1::Enemy1()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 30;
    reloadTicks = 20;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 1;
    dx = -speed + App::getInstance().getRandom().nextDouble(0.2) - 0.1;
    dy = App::getInstance().getRandom().nextDouble(0.2) - 0.1;
    width = 40;
    height = 32;
    texture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/enemy1.png");
    weapon = App::getInstance().getWeapon("EnemyWeapon1");
    name = "Enemy Fighter";
    onSpawn = [&](Entity* self) {
        App::getInstance().enemyCounter[1]++;
    };
    afterTick = [&](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            double deg = CommonUtil.getDegreeBetween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    App::getInstance().getPlayer()->x + App::getInstance().getPlayer()->width / 2,
                    App::getInstance().getPlayer()->y + App::getInstance().getPlayer()->height / 2
            );
            if (deg >= 150 || deg <= -150)
            {
                self->weapon->fire(self, deg);
                self->reloadTicks = self->weapon->reloadTicks;
            }
        }
    };
    onDeath = [&](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
        {
            App::getInstance().soundManager->playSound("assets/projectnuma/sounds/entity/enemyDie.wav");
            App::getInstance().getSession().credit += 5;
            App::getInstance().killboardLevel[1]++;
        }
        App::getInstance().enemyCounter[1]--;
    };
}

Enemy2::Enemy2()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 60;
    reloadTicks = 20;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 1;
    dx = -speed + App::getInstance().getRandom().nextDouble(0.2) - 0.1;
    dy = App::getInstance().getRandom().nextDouble(0.2) - 0.1;
    width = 60;
    height = 40;
    texture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/enemy2.png");
    weapon = App::getInstance().getWeapon("EnemyWeapon1");
    name = "Enemy armored fighter";
    onSpawn = [&](Entity* self) {
        App::getInstance().enemyCounter[2]++;
    };
    afterTick = [&](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            double deg = CommonUtil.getDegreeBetween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    App::getInstance().getPlayer()->x + App::getInstance().getPlayer()->width / 2,
                    App::getInstance().getPlayer()->y + App::getInstance().getPlayer()->height / 2
            );
            if (deg >= 135 || deg <= -135)
            {
                self->weapon->fire(self, deg);
                self->reloadTicks = self->weapon->reloadTicks;
            }
        }
    };
    onDeath = [&](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
        {
            App::getInstance().soundManager->playSound("assets/projectnuma/sounds/entity/enemyDie.wav");
            App::getInstance().getSession().credit += 10;
            App::getInstance().killboardLevel[2]++;
        }
        App::getInstance().enemyCounter[2]--;
    };
}

Enemy3::Enemy3()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 750;
    reloadTicks = 1000;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 0.25;
    dx = -speed;
    width = 256;
    height = 160;
    texture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/enemy3.png");
    weapon = App::getInstance().getWeapon("EnemyWeapon2");
    name = "Enemy cruiser";
    onSpawn = [&](Entity* self) {
        App::getInstance().soundManager->playSound("assets/projectnuma/sounds/ambient/warn.wav");
        self->isInvincible = true;
        App::getInstance().enemyCounter[3]++;
    };
    afterTick = [&](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            self->isInvincible = false;
            double deg = CommonUtil.getDegreeBetween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    App::getInstance().getPlayer()->x + App::getInstance().getPlayer()->width / 2,
                    App::getInstance().getPlayer()->y + App::getInstance().getPlayer()->height / 2
            );
            self->weapon->fire(self, deg);
            self->reloadTicks = self->weapon->reloadTicks;
        }
    };
    onDeath = [&](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
        {
            App::getInstance().soundManager->playSound("assets/projectnuma/sounds/entity/bossDie.wav");
            App::getInstance().getSession().credit += 50;
            App::getInstance().killboardLevel[3]++;
        }
        App::getInstance().enemyCounter[3]--;
    };
}
