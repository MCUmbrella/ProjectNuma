//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_ENTITY_H
#define PROJECTNUMA_ENTITY_H

#include "GeneralDefinitions.h"

/**
 * public class Entity
 */
class Entity
{
public:
    EntityType type = ENTITY_TYPE_OTHER;
    bool isControllable = false, isDead = false, isInvincible = false;
    int side = SIDE_NONE, maxHp = 0, hp = 0, reloadTicks = 0;
    double x = 0, y = 0, speed = 0, dx = 0, dy = 0;
    int width = 0, height = 0;
    SDL_Texture* texture = null;
    Weapon* weapon = null;
    string name;

    Entity(EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int hp, double speed,
           int side);

    Entity() = default;

    Entity* move(double x1, double y1);

    Entity* setLocation(double x1, double y1);

    /**
     * Move the entity, check its collisions and HP.
     */
    void tick();

    /**
     * The codes to be executed before the tick() function.
     */
    function<void(Entity* self)> beforeTick = null;

    /**
     * The codes to be executed after the tick() function.
     */
    function<void(Entity* self)> afterTick = null;

    /**
     * The code to be executed when the entity is added to the entities list.
     */
    function<void(Entity* self)> onSpawn = null;

    /**
     * The code to be executed when the entity was killed.
     */
    function<void(Entity* self)> onDeath = null;
};

#endif //PROJECTNUMA_ENTITY_H
