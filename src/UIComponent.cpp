//
// Created by mcumbrella on 23-5-8.
//
#include "UIComponent.h"

UIComponent* UIComponent::setTexture(SDL_Texture* newTexture, bool freeOldTexture)
{
    if (freeOldTexture)
        SDL_DestroyTexture(texture);
    texture = newTexture;
    return this;
}

UIComponent* UIComponent::setLocation(int newX, int newY)
{
    x = newX;
    y = newY;
    return this;
}

UIComponent::UIComponent(string name, SDL_Texture* t, int x, int y, bool freeTextureOnDestruct)
{
    this->name = std::move(name);
    texture = t;
    this->x = x;
    this->y = y;
    this->freeTextureOnDestruct = freeTextureOnDestruct;
}
