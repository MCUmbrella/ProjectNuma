//
// Created by mcumbrella on 23-5-9.
//

#include "Enemy1.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../Player.h"
#include "../../Session.h"
#include "../../weapon/Weapon.h"

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
