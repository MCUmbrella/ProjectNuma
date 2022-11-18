//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_RENDERMANAGER_H
#define PROJECTNUMA_RENDERMANAGER_H

static SDL_Texture* PLACEHOLDER_TEXTURE;
static map<const char*, SDL_Texture*> textures;
static SDL_Renderer* renderer = null;
static SDL_Window* window = null;
static TTF_Font* font = null;
const static int
        RENDERER_FLAGS = SDL_RENDERER_ACCELERATED,
        WINDOW_FLAGS = 0;

static class
{
private:
    static SDL_Texture* surfaceToTexture(SDL_Surface* surface, bool destroySurface)
    {
        SDL_Texture* texture;
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (destroySurface)
            SDL_FreeSurface(surface);
        return texture;
    }

public:
    static int getTextureWidth(SDL_Texture* texture)
    {
        if (texture == null) throw runtime_error("Null pointer passed to getTextureWidth()");
        int w;
        SDL_QueryTexture(texture, null, null, &w, null);
        return w;
    }

    static int getTextureHeight(SDL_Texture* texture)
    {
        if (texture == null) throw runtime_error("Null pointer passed to getTextureHeight()");
        int h;
        SDL_QueryTexture(texture, null, null, null, &h);
        return h;
    }

    static SDL_Texture* loadTexture(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading texture: %s", fileName);
        SDL_Texture* texture = IMG_LoadTexture(renderer, fileName);
        if (texture == null) throw runtime_error(string("Texture not found: ").append(fileName));
        textures[fileName] = texture;
        return texture;
    }

    static SDL_Texture* getTexture(const char* name)
    {
        SDL_Texture* t = textures[name];
        if (t == null) throw runtime_error(string("Texture not found: ").append(name));
        return t;
    }

    static void placeTexture(SDL_Texture* texture, int x, int y, int width, int height)
    {
        if (texture == null) throw runtime_error("Null pointer passed to placeTexture()");
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = width;
        destRect.h = height;
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    static void placeTexture(SDL_Texture* texture, int x, int y)
    {
        if (texture == null) throw runtime_error("Null pointer passed to placeTexture()");
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        SDL_QueryTexture(texture, null, null, &destRect.w, &destRect.h);
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    static SDL_Texture* getText(const char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        if (text == null) throw runtime_error("Null pointer passed to getText()");
        SDL_Color color;
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
        SDL_Surface* surface;
        surface = TTF_RenderUTF8_Blended(font, text, color);
        return surfaceToTexture(surface, true);
    }

    static void init()
    {
        // initialize sdl
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }
        if (TTF_Init() < 0)
        {
            printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing render manager");
        window = SDL_CreateWindow(VERSION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                  WINDOW_HEIGHT, WINDOW_FLAGS);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        for (const char* a: textureFiles)
            loadTexture(a);
        PLACEHOLDER_TEXTURE = getTexture("assets/projectnuma/textures/misc/placeholder.png");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading font: assets/projectnuma/font/MesloLGS_NF_Regular.ttf");
        font = TTF_OpenFont("assets/projectnuma/font/MesloLGS_NF_Regular.ttf", 24);
        if (font == null) throw runtime_error("Font not found: assets/projectnuma/font/MesloLGS_NF_Regular.ttf");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager initialization completed");
    }

    static void shutdown()
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager is being shut down now");
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager is down");
    }
} RenderManager;

#endif //PROJECTNUMA_RENDERMANAGER_H
