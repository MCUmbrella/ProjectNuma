#include "ProjectNuma.h"
#include "CommonUtil.h"
#include "RandomUtil.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "SessionUtil.h"

// import static
#define playSound SoundManager.playSound
#define setBGM SoundManager.setBGM
#define placeTexture RenderManager.placeTexture

static App* app;
static Session session;
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
    bool isControllable = false, isDead = false, isInvincible = false;
    int side = SIDE_NONE, maxHp = 0, hp = 0, reloadTicks = 0;
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

    void (* onSpawn)(Entity* self) = null;

    void (* onDeath)(Entity* self) = null;
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
        bulletDamage = 10;
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
        bulletDamage = 10;
        reloadTicks = 100;
        bulletTexture = PLACEHOLDER_TEXTURE;
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * PlayerWeapon1.class
 * extends Weapon
 */
class PlayerWeapon1 : public Weapon
{
public:
    PlayerWeapon1()
    {
        bulletSpeed = 20;
        bulletDamage = 5;
        reloadTicks = 20;
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
    shared_ptr<Player> player;
    std::map<const char*, Weapon*> weapons;
    std::list<shared_ptr<Entity>> entities;
    std::list<shared_ptr<Entity>> environment;
    std::list<shared_ptr<Entity>> ui;

public:
    bool pressedKey[1024]{};

    void startup();

    void shutdown();

    SDL_Renderer* getRenderer();

    Weapon* getWeapon(const char* name);

    Entity* getPlayer();

    std::list<shared_ptr<Entity>> getEntities();

    Entity* addEntity(Entity* e);

    void removeEntity(Entity* e, bool callOnDeath);

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

Entity::Entity(const EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int maxHP, double speed,
               int side)
{
    this->type = type;
    this->name = std::string(name);
    this->texture = texture;
    this->weapon = weapon;
    this->hp = this->maxHp = maxHP;
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
    // tickBefore
    if (customTickBefore != null) customTickBefore(this);
    // general tick operations
    move(dx, dy);
    if (weapon != null && reloadTicks > 0) reloadTicks--;
    // check if this is bullet and hits something
    if (type == ENTITY_TYPE_BULLET)
    {
        for (auto& e: app->getEntities())
        {
            if (!e->isDead && e->type != ENTITY_TYPE_BULLET && e->side != side && !e->isInvincible && CommonUtil.checkCollision(
                    e->x, e->y, e->width, e->height, x, y, width, height
            ))
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s collision with %s", e->name.c_str(), name.c_str());
                this->hp = 0;
                e->hp -= ((Bullet*) this)->damage;
                playSound("assets/projectnuma/sounds/entity/hit.wav");
                break;
            }
        }
    }
    // tickAfter or not
    if (hp <= 0)
    {
        if (type != ENTITY_TYPE_BULLET)SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s dies", name.c_str());
        if (onDeath != null) onDeath(this);
        isDead = true;
    }
    else if (customTickAfter != null)
        customTickAfter(this);
}

// PlayerWeapon0 =======================================================================================================

void PlayerWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Default weapon fire");
    degree += r.nextDouble(6.0) - 3.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->texture = bulletTexture;
    app->addEntity(b);
    playSound("assets/projectnuma/sounds/item/weapon0.wav");
}

// EnemyWeapon0 ========================================================================================================

void EnemyWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy default weapon fire");
    degree += r.nextDouble(2.0) - 1.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->hp = 100;
    b->texture = bulletTexture;
    b->customTickAfter = [](Entity* self) {
        self->hp--;
    };
    app->addEntity(b);
    playSound("assets/projectnuma/sounds/item/weapon0e.wav");
}

// PlayerWeapon1 =======================================================================================================

void PlayerWeapon1::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Weapon 1 fire");
    degree += r.nextDouble(6.0) - 3.0;
    for (int i = -2; i != 3; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 8, 8,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 3
        );
        b->texture = bulletTexture;
        app->addEntity(b);
    }
    playSound("assets/projectnuma/sounds/item/weapon1.wav");
}

// App =================================================================================================================

void App::startup()
{
    app = this;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is coming up. Please wait");
    RenderManager.init();
    SoundManager.init();
    setBGM("assets/projectnuma/sounds/music/game/0.ogg");
    // initialize weapons
    weapons.emplace("PlayerWeapon0", new PlayerWeapon0());
    weapons.emplace("EnemyWeapon0", new EnemyWeapon0());
    weapons.emplace("PlayerWeapon1", new PlayerWeapon1());
    // initialize player
    player = make_shared<Player>(Player());
    addEntity(player.get());
    // load session
    session.load();
    player->maxHp = session.hp;
    player->hp = session.hp;
    // completed
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is ready. Entering main loop");
    mainTickLoop();
}

void App::shutdown()
{
    // shutdown sdl
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shutting down");
    IMG_Quit();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    // save session
    session.hp = player->maxHp;
    session.save();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Completed");
    exit(0);
}

SDL_Renderer* App::getRenderer()
{
    return renderer;
}

Weapon* App::getWeapon(const char* name)
{
    Weapon* w = weapons[name];
    if (w == null) throw std::runtime_error(string("Weapon not found: ").append(name));
    return w;
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
    if (e->onSpawn != null) e->onSpawn(e);
    return e;
}

void App::removeEntity(Entity* e, bool callOnDeath) //FIXME: crashes on call
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Remove entity %s", e->name.c_str());
    for (const shared_ptr<Entity>& ee: entities)
    {
        if (ee.get() == e)
        {
            if (callOnDeath && e->onDeath != null) e->onDeath(e);
            entities.remove(ee);
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
            if (e->type != ENTITY_TYPE_PLAYER && !e->isDead)
                e->tick();
            else if (e->type == ENTITY_TYPE_PLAYER) e->tick();
        }
        // refresh screen
        SDL_SetRenderDrawColor(renderer, 64, 64, 128, 255);
        SDL_RenderClear(renderer);
        // place textures
        for (const shared_ptr<Entity>& e: entities)
        {
            if (!e->isDead) placeTexture(e->texture, e->x, e->y, e->width, e->height);
        }
        SDL_RenderPresent(renderer);
        // clean up
        if (tick % 100 == 0)
            entities.remove_if([](const shared_ptr<Entity>& e) { return e->isDead && e->type != ENTITY_TYPE_PLAYER; });
        SDL_Delay(16);
    }
}

// Player ==============================================================================================================

Player::Player()
{
    type = ENTITY_TYPE_PLAYER;
    isControllable = true;
    side = SIDE_PLAYER;
    maxHp = 100;
    hp = 50;
    speed = 6;
    width = 40;
    height = 32;
    x = 100;
    y = WINDOW_HEIGHT / 2 - height / 2;
    texture = PLACEHOLDER_TEXTURE;
    weapon = app->getWeapon("PlayerWeapon0");
    name = "Player";
    customTickBefore = [](Entity* self) {
        if (self->isDead)
        {
            self->hp++;
            if (self->hp == self->maxHp)
            {
                self->isDead = false;
                self->x = 100;
                self->y = WINDOW_HEIGHT / 2 - self->height / 2;
            }
            return;
        }
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
    onSpawn = [](Entity* self) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player spawned");
    };
    onDeath = [](Entity* self) {
        playSound("assets/projectnuma/sounds/entity/playerDie.wav");
    };
}

// Enemy0 ==============================================================================================================

Enemy0::Enemy0() : Entity()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = 10;
    hp = 10;
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
    onDeath = [](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
            playSound("assets/projectnuma/sounds/entity/hit.wav");
        session.credit++;
    };
}

// Bullet ==============================================================================================================

Bullet::Bullet(Entity* owner, int damage, int width, int height, double x, double y, double speed, double facing)
{
    this->damage = damage;
    type = ENTITY_TYPE_BULLET;
    side = owner->side;
    maxHp = 1;
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
