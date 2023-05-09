//
// Created by mcumbrella on 22-11-17.
//

#ifndef PROJECTNUMA_UICOMPONENT_H
#define PROJECTNUMA_UICOMPONENT_H

#include "GeneralDefinitions.h"

class UIComponent
{
public:
    bool freeTextureOnDestruct = false;
    int x = 0, y = 0;
    SDL_Texture* texture = null;
    string name;

    UIComponent* setTexture(SDL_Texture* newTexture, bool freeOldTexture);

    UIComponent* setLocation(int newX, int newY);

    UIComponent(string name, SDL_Texture* t, int x, int y, bool freeTextureOnDestruct);

    ~UIComponent()
    {
        if (freeTextureOnDestruct)
            SDL_DestroyTexture(texture);
    }
};

#endif //PROJECTNUMA_UICOMPONENT_H
