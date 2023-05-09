//
// Created by mcumbrella on 23-5-9.
//

#include "Enemy2.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../Player.h"
#include "../../Session.h"
#include "../../weapon/Weapon.h"

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
            double deg = CommonUtil::getDegreeBetween(
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
