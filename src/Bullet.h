//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_BULLET_H
#define PROJECTNUMA_BULLET_H

#include "Entity.h"

/**
 * public class Bullet extends Entity
 */
class Bullet : public Entity
{
public:
    int damage;
    Entity* owner = null;

    Bullet(Entity* owner, int damage, int width, int height, double x, double y, double speed, double facing);
};

#endif //PROJECTNUMA_BULLET_H
