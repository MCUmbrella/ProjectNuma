#include "ProjectNuma.h"
#include "CommonUtil.h"
#include "RandomUtil.h"

static SDL_Texture* PLACEHOLDER_TEXTURE;
static App* app;
static Random r;

using namespace std;

// CLASS DEFINITIONS START =============================================================================================

/**
 * Entity.class
 */
class Entity
{
public:
    EntityType type = ENTITY_TYPE_OTHER;
    bool isControllable = false, isDead = false;
    int side = SIDE_NONE, hp = 0, reloadTicks = 0;
    double x = 0, y = 0, speed = 0, dx = 0, dy = 0;
    int width = 0, height = 0;
    SDL_Texture* texture = null;
    Weapon* weapon = null;
    std::string name;

    Entity(EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int hp, double speed,
           int side);

    Entity() = default;

    Entity* move(double x1, double y1);

    Entity* setLocation(double x1, double y1);

    void tick();

    void (* customTickBefore)(Entity* self) = null;

    void (* customTickAfter)(Entity* self) = null;
};

/**
 * Weapon.class
 */
class Weapon
{
public:
    int bulletDamage, reloadTicks;
    double bulletSpeed;
    SDL_Texture* bulletTexture;

    virtual void fire(Entity* owner, double degree) = 0;
};

/**
 * PlayerWeapon0.class
 * extends Weapon
 */
class PlayerWeapon0 : public Weapon
{
public:
    PlayerWeapon0()
    {
        bulletSpeed = 20;
        bulletDamage = 1;
        reloadTicks = 10;
        bulletTexture = PLACEHOLDER_TEXTURE;
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * EnemyWeapon0.class
 * extends Weapon
 */
class EnemyWeapon0 : public Weapon
{
public:
    EnemyWeapon0()
    {
        bulletSpeed = 10;
        bulletDamage = 1;
        reloadTicks = 100;
        bulletTexture = PLACEHOLDER_TEXTURE;
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * App.class
 */
class App
{
private:
    const int
            RENDERER_FLAGS = SDL_RENDERER_ACCELERATED,
            WINDOW_FLAGS = 0;

    SDL_Renderer* renderer = null;
    SDL_Window* window = null;
    shared_ptr<Player> player;
    std::map<const char*, SDL_Texture*> textures;
    std::map<const char*, Weapon*> weapons;
    std::list<shared_ptr<Entity>> entities;

public:
    bool pressedKey[1024]{};

    void startup();

    void shutdown();

    SDL_Renderer* getRenderer();

    SDL_Texture* loadTexture(const char* fileName);

    SDL_Texture* getTexture(const char* name);

    Weapon* getWeapon(const char* name);

    Entity* getPlayer();

    std::list<shared_ptr<Entity>> getEntities();

    Entity* addEntity(Entity* e);

    void removeEntity(Entity* e);

    void mainTickLoop();
};

/**
 * Player.class
 * extends Entity
 */
class Player : public Entity
{
public:
    Player();
};

/**
 * Enemy0.class
 * extends Entity
 */
class Enemy0 : public Entity
{
public:
    Enemy0();
};

/**
 * Enemy0.class
 * extends Entity
 */
class Bullet : public Entity
{
public:
    int damage;
    Entity* owner = null;

    Bullet(Entity* owner, int damage, int width, int height, double x, double y, double speed, double facing);
};
// CLASS DEFINITIONS END ===============================================================================================

// FUNCTION DEFINITIONS START ==========================================================================================

// Entity ==============================================================================================================

Entity::Entity(const EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int hp, double speed,
               int side)
{
    this->type = type;
    this->name = std::string(name);
    this->texture = texture;
    this->weapon = weapon;
    this->hp = hp;
    this->speed = speed;
    this->side = side;
    SDL_QueryTexture(texture, null, null, &width, &height);
}

Entity* Entity::move(double x1, double y1)
{
    x += x1;
    y += y1;
    return this;
}

Entity* Entity::setLocation(double x1, double y1)
{
    this->x = x1;
    this->y = y1;
    return this;
}

void Entity::tick()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s tickBefore", name.c_str());
    if (customTickBefore != null) customTickBefore(this);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s tick", name.c_str());
    // general tick operations
    move(dx, dy);
    if (weapon != null && reloadTicks > 0) reloadTicks--;
    // check if this is bullet and hits something
    if (type == ENTITY_TYPE_BULLET)
    {
        for (auto& e: app->getEntities())
        {
            if (!e->isDead && e->type != ENTITY_TYPE_BULLET && e->side != side && CommonUtil.checkCollision(
                    e->x, e->y, e->width, e->height, x, y, width, height
            ))
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s collision with %s", e->name.c_str(), name.c_str());
                this->hp = 0;
                e->hp -= ((Bullet*) this)->damage;
                break;
            }
        }
    }
    if (hp <= 0)
    {
        if (type != ENTITY_TYPE_BULLET)SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s dies", name.c_str());
        isDead = true;
    }
    else if (customTickAfter != null)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s tickAfter", name.c_str());
        customTickAfter(this);
    }
}

// PlayerWeapon0 =======================================================================================================

void PlayerWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Default weapon fire");
    degree += r.nextDouble(6.0) - 3.0;
    Bullet* b = new Bullet(
            owner, 1, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->texture = app->getTexture("assets/projectnuma/textures/misc/black.png");
    app->addEntity(b);
}

// EnemyWeapon0 ========================================================================================================

void EnemyWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy default weapon fire");
    degree += r.nextDouble(2.0) - 1.0;
    Bullet* b = new Bullet(
            owner, 1, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->hp = 100;
    b->texture = app->getTexture("assets/projectnuma/textures/misc/black.png");
    b->customTickAfter = [](Entity* self) {
        self->hp--;
    };
    app->addEntity(b);
}

// App =================================================================================================================

void App::startup()
{
    app = this;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initializing");
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, WINDOW_FLAGS);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    // initialize textures
    loadTexture("assets/projectnuma/textures/misc/black.png");
    PLACEHOLDER_TEXTURE = loadTexture("assets/projectnuma/textures/misc/placeholder.png");
    // initialize weapons
    weapons.emplace("PlayerWeapon0", new PlayerWeapon0());
    weapons.emplace("EnemyWeapon0", new EnemyWeapon0());
    // initialize player
    player = make_shared<Player>(Player());
    addEntity(player.get());
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialization completed. Entering main loop");
    mainTickLoop();
}

void App::shutdown()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shutting down");
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Completed");
    exit(0);
}

SDL_Renderer* App::getRenderer()
{
    return renderer;
}

SDL_Texture* App::loadTexture(const char* fileName)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading texture %s", fileName);
    SDL_Texture* texture = IMG_LoadTexture(renderer, fileName);
    assert(texture != null);
    textures[fileName] = texture;
    return texture;
}

SDL_Texture* App::getTexture(const char* name)
{
    return textures[name];
}

Weapon* App::getWeapon(const char* name)
{
    return weapons[name];
}

Entity* App::getPlayer()
{
    return player.get();
}

std::list<shared_ptr<Entity>> App::getEntities()
{
    return entities;
}

Entity* App::addEntity(Entity* e)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Add entity %s", e->name.c_str());
    entities.emplace_back(e);
    return e;
}

void App::removeEntity(Entity* e)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Remove entity %s", e->name.c_str());
    for (shared_ptr<Entity> ee: entities)
    {
        if (ee.get() == e)
        {
            entities.remove(ee);
            free(e);
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entity removed");
            return;
        }
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Entity not found");
}

void App::mainTickLoop()
{
    for (unsigned long long tick = 0;; tick++)
    {
        // tick SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    shutdown();
                case SDL_KEYDOWN:
                {
                    SDL_KeyboardEvent e = event.key;
                    if (e.repeat == 0)
                        pressedKey[e.keysym.scancode] = true;
                    break;
                }
                case SDL_KEYUP:
                {
                    SDL_KeyboardEvent e = event.key;
                    if (e.repeat == 0)
                        pressedKey[e.keysym.scancode] = false;
                    break;
                }
                default:
                    break;
            }
        }
        // randomly add enemies
        if (r.nextInt(100) == 0) addEntity((new Enemy0())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 24)));
        // tick entities
        for (const shared_ptr<Entity>& e: entities)
        {
            if (!e->isDead)
            {
                e->tick();
            }
        }
        // refresh screen
        SDL_SetRenderDrawColor(renderer, 64, 64, 128, 255);
        SDL_RenderClear(renderer);
        // place textures
        for (const shared_ptr<Entity>& e: entities)
        {
            if (!e->isDead) CommonUtil.placeTexture(renderer, e->texture, e->x, e->y, e->width, e->height);
        }
        SDL_RenderPresent(renderer);
        // clean up
        if (tick % 100 == 0)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Cleaning up (%zu entities)", entities.size());
            entities.remove_if([](const shared_ptr<Entity>& e) { return e->isDead && e->type != ENTITY_TYPE_PLAYER; });
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Cleanup completed. %zu entites left", entities.size());
        }
        SDL_Delay(16);
    }
}

// Player ==============================================================================================================

Player::Player()
{
    type = ENTITY_TYPE_PLAYER;
    isControllable = true;
    side = SIDE_PLAYER;
    hp = 1000000000;
    speed = 6;
    width = 40;
    height = 32;
    texture = PLACEHOLDER_TEXTURE;
    weapon = app->getWeapon("PlayerWeapon0");
    name = "Player";
    customTickBefore = [](Entity* self) {
        self->dx = self->dy = 0;
        if (app->pressedKey[SDL_SCANCODE_W])
            self->dy = -self->speed;
        if (app->pressedKey[SDL_SCANCODE_A])
            self->dx = -self->speed;
        if (app->pressedKey[SDL_SCANCODE_S])
            self->dy = self->speed;
        if (app->pressedKey[SDL_SCANCODE_D])
            self->dx = self->speed;
        if (self->weapon != null && app->pressedKey[SDL_SCANCODE_SPACE] && self->reloadTicks == 0)
        {
            self->weapon->fire(self, 0);
            self->reloadTicks = self->weapon->reloadTicks;
        }
    };
    customTickAfter = [](Entity* self) {
        self->setLocation(
                self->x + self->width > WINDOW_WIDTH ? WINDOW_WIDTH - self->width :
                self->x < 0 ? 0 :
                self->x,
                self->y + self->height > WINDOW_HEIGHT ? WINDOW_HEIGHT - self->height :
                self->y < 0 ? 0 :
                self->y
        );
    };
}

// Enemy0 ==============================================================================================================

Enemy0::Enemy0() : Entity()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    hp = 1;
    reloadTicks = 10;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 2;
    dx = -speed + r.nextDouble(2.0) - 1.0;
    dy = r.nextDouble(1.0) - 0.5;
    width = 32;
    height = 24;
    texture = PLACEHOLDER_TEXTURE;
    weapon = app->getWeapon("EnemyWeapon0");
    name = "Enemy0";
    customTickAfter = [](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            double deg = CommonUtil.getDegreeBeteween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    app->getPlayer()->x + app->getPlayer()->width / 2,
                    app->getPlayer()->y + app->getPlayer()->height / 2
            );
            self->weapon->fire(self, deg);
            self->reloadTicks = self->weapon->reloadTicks;
        }
    };
}

// Bullet ==============================================================================================================

Bullet::Bullet(Entity* owner, int damage, int width, int height, double x, double y, double speed, double facing)
{
    this->damage = damage;
    type = ENTITY_TYPE_BULLET;
    side = owner->side;
    hp = 1;
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->dx = speed * cos(facing * 0.0174533);
    this->dy = speed * -sin(facing * 0.0174533);
    this->width = width;
    this->height = height;
    texture = PLACEHOLDER_TEXTURE;
    name = "Bullet of " + owner->name;
    customTickBefore = [](Entity* self) {
        if (self->x + self->width <= 0 || self->y + self->height <= 0 || self->x >= WINDOW_WIDTH || self->y >= WINDOW_HEIGHT)
        {
            self->hp = 0;
            self->isDead = true;
        }
    };
}
// FUNCTION DEFINITIONS END ============================================================================================

int main(int argc, char** argv)
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    (new App())->startup();
}
