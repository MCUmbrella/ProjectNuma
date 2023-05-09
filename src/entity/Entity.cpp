//
// Created by mcumbrella on 23-5-8.
//

#include "Entity.h"
#include "../App.h"
#include "../CommonUtil.h"
#include "../SoundManager.h"
#include "Player.h"
#include "Bullet.h"

Entity::Entity(const EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int maxHP, double speed,
               int side)
{
    this->type = type;
    this->name = string(name);
    this->texture = texture;
    this->weapon = weapon;
    this->hp = this->maxHp = maxHP;
    this->speed = speed;
    this->side = side;
    SDL_QueryTexture(texture, null, null, &width, &height);
}

Entity* Entity::move(double x1, double y1)
{
    x += x1;
    y += y1;
    return this;
}

Entity* Entity::setLocation(double x1, double y1)
{
    this->x = x1;
    this->y = y1;
    return this;
}

void Entity::tick()
{
    // tickBefore
    if (beforeTick != null) beforeTick(this);

    // general tick operations
    move(dx, dy);
    if (weapon != null && reloadTicks > 0) reloadTicks--;
    if (type == ENTITY_TYPE_BULLET)
    {
        // this is bullet and hits something
        for (auto& e : App::getInstance().getEntities())
        {
            if (!e->isDead && e->type != ENTITY_TYPE_BULLET && e->side != side && !e->isInvincible && CommonUtil.checkCollision(
                    e->x, e->y, e->width, e->height, x, y, width, height
            ))
            {
                this->hp = 0;
                e->hp -= ((Bullet*) this)->damage;
                App::getInstance().soundManager->playSound("assets/projectnuma/sounds/entity/hit.wav");
                break;
            }
        }
    }
    else if (type != ENTITY_TYPE_PLAYER && side != SIDE_PLAYER && CommonUtil.checkCollision(
            App::getInstance().getPlayer()->x, App::getInstance().getPlayer()->y, App::getInstance().getPlayer()->width,
            App::getInstance().getPlayer()->height,
            x, y, width, height
    ) && !App::getInstance().getPlayer()->isDead && (!isInvincible && !App::getInstance().getPlayer()->isInvincible))
    {
        // this is enemy and has collision with player
        int hp0 = hp, hp1 = App::getInstance().getPlayer()->hp;
        hp -= hp1;
        App::getInstance().getPlayer()->hp -= hp0;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s collides with player", name.c_str());
    }

    // tickAfter or not
    if (hp <= 0)
    {
        if (type != ENTITY_TYPE_BULLET)SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s dies", name.c_str());
        if (onDeath != null) onDeath(this);
        isDead = true;
    }
    else if (afterTick != null)
        afterTick(this);
}
