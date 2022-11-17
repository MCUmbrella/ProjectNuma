//
// Created by mcumbrella on 22-11-17.
//

#ifndef PROJECTNUMA_UICOMPONENT_H
#define PROJECTNUMA_UICOMPONENT_H

#include <SDL_render.h>
#include <memory>
#include <utility>
#include "CommonUtil.h"

class UIComponent
{
public:
    SDL_Texture* texture = null;
    int x = 0, y = 0;
    std::string name;

    UIComponent* setTexture(SDL_Texture* newTexture, bool freeOldTexture)
    {
        if(freeOldTexture)
            SDL_DestroyTexture(texture);
        texture = newTexture;
        return this;
    }

    UIComponent* setLocation(int newX, int newY)
    {
        x = newX;
        y = newY;
        return this;
    }

    UIComponent(std::string name, SDL_Texture* t, int x, int y)
    {
        this->name = std::move(name);
        texture = t;
        this->x = x;
        this->y = y;
    }
};

#endif //PROJECTNUMA_UICOMPONENT_H
