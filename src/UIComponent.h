//
// Created by mcumbrella on 22-11-17.
//

#ifndef PROJECTNUMA_UICOMPONENT_H
#define PROJECTNUMA_UICOMPONENT_H

class UIComponent
{
public:
    bool freeTextureOnDestruct = false;
    int x = 0, y = 0;
    SDL_Texture* texture = null;
    string name;

    UIComponent* setTexture(SDL_Texture* newTexture, bool freeOldTexture)
    {
        if (freeOldTexture)
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

    UIComponent(string name, SDL_Texture* t, int x, int y, bool freeTextureOnDestruct)
    {
        this->name = std::move(name);
        texture = t;
        this->x = x;
        this->y = y;
        this->freeTextureOnDestruct = freeTextureOnDestruct;
    }

    ~UIComponent()
    {
        if (freeTextureOnDestruct)
            SDL_DestroyTexture(texture);
    }
};

#endif //PROJECTNUMA_UICOMPONENT_H
