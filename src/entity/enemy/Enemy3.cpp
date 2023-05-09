//
// Created by mcumbrella on 23-5-9.
//

#include "Enemy3.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../Player.h"
#include "../../Session.h"
#include "../../weapon/Weapon.h"

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
