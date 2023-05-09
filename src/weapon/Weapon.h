//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_WEAPON_H
#define PROJECTNUMA_WEAPON_H

#include "../GeneralDefinitions.h"

/**
 * public class Weapon
 */
class Weapon
{
public:
    int bulletDamage, reloadTicks;
    double bulletSpeed;
    string name = "(unnamed weapon)";
    SDL_Texture* bulletTexture = null;

    /**
     *
     * @param owner The pointer to the owner of this weapon.
     * @param degree The angle of the bullet. 0 degree is the right side.
     */
    virtual void fire(Entity* owner, double degree) = 0;
};

#endif //PROJECTNUMA_WEAPON_H
