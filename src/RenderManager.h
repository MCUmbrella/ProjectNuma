//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_RENDERMANAGER_H
#define PROJECTNUMA_RENDERMANAGER_H

#include <SDL_render.h>

static SDL_Texture* PLACEHOLDER_TEXTURE;
static std::map<const char*, SDL_Texture*> textures;
static SDL_Renderer* renderer = null;
static SDL_Window* window = null;
const static int
        RENDERER_FLAGS = SDL_RENDERER_ACCELERATED,
        WINDOW_FLAGS = 0;

static class
{
public:
    static int getTextureWidth(SDL_Texture* texture)
    {
        if (texture == null) throw std::runtime_error("Null pointer");
        int w;
        SDL_QueryTexture(texture, null, null, &w, null);
        return w;
    }

    static int getTextureHeight(SDL_Texture* texture)
    {
        if (texture == null) throw std::runtime_error("Null pointer");
        int h;
        SDL_QueryTexture(texture, null, null, null, &h);
        return h;
    }

    static SDL_Texture* loadTexture(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading texture: %s", fileName);
        SDL_Texture* texture = IMG_LoadTexture(renderer, fileName);
        if (texture == null) throw std::runtime_error(std::string("Texture not found: ").append(fileName));
        textures[fileName] = texture;
        return texture;
    }

    static SDL_Texture* getTexture(const char* name)
    {
        SDL_Texture* t = textures[name];
        if (t == null) throw std::runtime_error(std::string("Texture not found: ").append(name));
        return t;
    }

    static void placeTexture(SDL_Texture* texture, int x, int y, int width, int height)
    {
        if (texture == null) throw std::runtime_error("Null pointer");
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = width;
        destRect.h = height;
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    static void placeTexture(SDL_Texture* texture, int x, int y)
    {
        if (texture == null) throw std::runtime_error("Null pointer");
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        SDL_QueryTexture(texture, null, null, &destRect.w, &destRect.h);
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    static void init()
    {
        // initialize sdl
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing render manager");
        window = SDL_CreateWindow("App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                  WINDOW_HEIGHT, WINDOW_FLAGS);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        for (const char* a: textureFiles)
            loadTexture(a);
        PLACEHOLDER_TEXTURE = getTexture("assets/projectnuma/textures/misc/placeholder.png");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager initialization completed");
    }

    static void shutdown()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager is being shut down now");
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager is down");
    }
} RenderManager;

#endif //PROJECTNUMA_RENDERMANAGER_H
