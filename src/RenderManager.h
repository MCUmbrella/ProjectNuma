//
// Created by mcumbrella on 22-11-13.
//

#ifndef PROJECTNUMA_RENDERMANAGER_H
#define PROJECTNUMA_RENDERMANAGER_H

static SDL_Texture* PLACEHOLDER_TEXTURE;
static SDL_Renderer* renderer = null;
static SDL_Window* window = null;

static class
{
private:
    map<const char*, SDL_Texture*> textures;
    TTF_Font* font16 = null;
    TTF_Font* font24 = null;
    TTF_Font* font32 = null;
    TTF_Font* font40 = null;
    TTF_Font* font48 = null;

public:
    static SDL_Texture* surfaceToTexture(SDL_Surface* surface, bool destroySurface)
    {
        SDL_Texture* texture;
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (destroySurface)
            SDL_FreeSurface(surface);
        return texture;
    }

    /**
     * Get the width of a texture.
     */
    static int getTextureWidth(SDL_Texture* texture)
    {
        if (texture == null) throw runtime_error("Null pointer passed to getTextureWidth()");
        int w;
        SDL_QueryTexture(texture, null, null, &w, null);
        return w;
    }

    /**
     * Get the height of a texture.
     */
    static int getTextureHeight(SDL_Texture* texture)
    {
        if (texture == null) throw runtime_error("Null pointer passed to getTextureHeight()");
        int h;
        SDL_QueryTexture(texture, null, null, null, &h);
        return h;
    }

    /**
     * Load a texture from file.
     * @param fileName The file path of the texture.
     */
    SDL_Texture* loadTexture(const char* fileName)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading texture: %s", fileName);
        SDL_Texture* texture = IMG_LoadTexture(renderer, fileName);
        if (texture == null) throw runtime_error(string("Texture not found: ") + fileName);
        textures[fileName] = texture;
        return texture;
    }

    /**
     * Get the loaded texture by its path.
     * @param name The path of the texture file.
     */
    SDL_Texture* getTexture(const char* name)
    {
        SDL_Texture* t = textures[name];
        if (t == null) throw runtime_error(string("Texture not found: ") + name);
        return t;
    }

    /**
     * Resize and place a texture on the screen.
     * @param texture The pointer to the texture.
     * @param width The width that needs to be stretched to.
     * @param height The height that needs to be stretched to.
     */
    void placeTexture(SDL_Texture* texture, int x, int y, int width, int height)
    {
        if (texture == null) throw runtime_error("Null pointer passed to placeTexture()");
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = width;
        destRect.h = height;
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    /**
     * Place a texture on the screen.
     * @param texture The pointer to the texture.
     */
    void placeTexture(SDL_Texture* texture, int x, int y)
    {
        if (texture == null) throw runtime_error("Null pointer passed to placeTexture()");
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        SDL_QueryTexture(texture, null, null, &destRect.w, &destRect.h);
        SDL_RenderCopy(renderer, texture, null, &destRect);
    }

    /**
     * Convert a text to the texture object.
     * @param text The text that needs to be converted.
     * @param r RGB value.
     * @param g RGB value.
     * @param b RGB value.
     * @param a Alpha value.
     * @param size FontSize: XS, S, M, L, XL.
     */
    SDL_Texture* textToTexture(const char* text, unsigned char r, unsigned char g, unsigned char b, unsigned char a, FontSize size)
    {
        if (text == null) throw runtime_error("Null pointer passed to textToTexture()");
        SDL_Color color;
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
        SDL_Surface* surface;
        surface = TTF_RenderUTF8_Blended(
                size == FONT_SIZE_XL ? font48 :
                size == FONT_SIZE_L ? font40 :
                size == FONT_SIZE_M ? font32 :
                size == FONT_SIZE_S ? font24 :
                font16,
                text, color
        );
        return surfaceToTexture(surface, true);
    }

    /**
     * Initialize the render manager.
     */
    void init()
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
        window = SDL_CreateWindow(VERSION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS | (flFullscreen ? SDL_WINDOW_FULLSCREEN : 0));
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS | (flSoftwareRender ? SDL_RENDERER_SOFTWARE : SDL_RENDERER_ACCELERATED));
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        for (const char* a : textureFiles)
            loadTexture(a);
        PLACEHOLDER_TEXTURE = getTexture("assets/projectnuma/textures/misc/placeholder.png");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading font: %s", FONT_FILE);
        font16 = TTF_OpenFont(FONT_FILE, 16);
        font24 = TTF_OpenFont(FONT_FILE, 24);
        font32 = TTF_OpenFont(FONT_FILE, 32);
        font40 = TTF_OpenFont(FONT_FILE, 40);
        font48 = TTF_OpenFont(FONT_FILE, 48);
        if (font16 == null) throw runtime_error(string("Font not found: ") + FONT_FILE);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render manager initialization completed");
    }

    /**
     * Shutdown the render manager.
     */
    void shutdown()
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
