//
// Created by mcumbrella on 23-5-9.
//

#include "Enemy0.h"
#include "../../App.h"
#include "../../Random.h"
#include "../../RenderManager.h"
#include "../../SoundManager.h"
#include "../Player.h"
#include "../../Session.h"
#include "../../weapon/Weapon.h"

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
            double deg = CommonUtil::getDegreeBetween(
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
