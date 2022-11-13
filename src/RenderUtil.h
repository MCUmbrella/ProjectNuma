//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_RENDERUTIL_H
#define PROJECTNUMA_RENDERUTIL_H

#include <SDL_render.h>

static class
{
public:
    static int getTextureWidth(SDL_Texture* texture)
    {
        int w;
        SDL_QueryTexture(texture, null, null, &w, null);
        return w;
    }

    static int getTextureHeight(SDL_Texture* texture)
    {
        int h;
        SDL_QueryTexture(texture, null, null, null, &h);
        return h;
    }

    static void placeTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height)
    {
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = width;
        destRect.h = height;
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    static void placeTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y)
    {
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        SDL_QueryTexture(texture, null, null, &destRect.w, &destRect.h);
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }
} RenderUtil;

#endif //PROJECTNUMA_RENDERUTIL_H
