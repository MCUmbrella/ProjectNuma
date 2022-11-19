//
// Created by mcumbrella on 22-11-8.
//

#ifndef PROJECTNUMA_PROJECTNUMA_H
#define PROJECTNUMA_PROJECTNUMA_H

#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <map>
#include <memory>
#include <list>
#include <stdexcept>
#include <string>
#include "Values.h"
#include "CommonUtil.h"
#include "RandomUtil.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "SessionUtil.h"
#include "UIComponent.h"
#include "LevelUtil.h"

typedef char EntityType;

// import static
#define playSound SoundManager.playSound
#define setBGM SoundManager.setBGM
#define getTexture RenderManager.getTexture
#define placeTexture RenderManager.placeTexture

class App;

class Entity;

class Player;

class Enemy0;

class Enemy1;

class Enemy2;

class Enemy3;

class Bullet;

class Weapon;

class PlayerWeapon0;

class PlayerWeapon1;

class PlayerWeapon2;

class PlayerWeapon3;

class EnemyWeapon0;

class EnemyWeapon1;

class EnemyWeapon2;

static App* app;
static Session session;
static Random r;

// CLASS DEFINITIONS START =============================================================================================

/**
 * public class Entity
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
    string name;

    Entity(EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int hp, double speed,
           int side);

    Entity() = default;

    Entity* move(double x1, double y1);

    Entity* setLocation(double x1, double y1);

    void tick();

    void (* beforeTick)(Entity* self) = null;

    void (* afterTick)(Entity* self) = null;

    void (* onSpawn)(Entity* self) = null;

    void (* onDeath)(Entity* self) = null;
};

/**
 * public class Player extends Entity
 */
class Player : public Entity
{
public:
    int life = 1, invincibleTicks = 150;
    double speedModifier = 1.0;
    vector<Weapon*> currentWeapons;

    Player();
};

/**
 * public class Enemy0 extends Entity
 */
class Enemy0 : public Entity
{
public:
    Enemy0();
};

/**
 * public class Enemy1 extends Entity
 */
class Enemy1 : public Entity
{
public:
    Enemy1();
};

/**
 * public class Enemy2 extends Entity
 */
class Enemy2 : public Entity
{
public:
    Enemy2();
};

/**
 * public class Enemy3 extends Entity
 */
class Enemy3 : public Entity
{
public:
    Enemy3();
};

/**
 * public class Bullet extends Entity
 */
class Bullet : public Entity
{
public:
    int damage;
    Entity* owner = null;

    Bullet(Entity* owner, int damage, int width, int height, double x, double y, double speed, double facing);
};

/**
 * public class Weapon
 */
class Weapon
{
public:
    int bulletDamage, reloadTicks;
    double bulletSpeed;
    string name = "(unnamed weapon)";
    SDL_Texture* bulletTexture;

    virtual void fire(Entity* owner, double degree) = 0;
};

/**
 * public class PlayerWeapon0 extends Weapon
 */
class PlayerWeapon0 : public Weapon
{
public:
    PlayerWeapon0()
    {
        name = "Small machine gun";
        bulletSpeed = 20;
        bulletDamage = 10;
        reloadTicks = 10;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon0bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon1 extends Weapon
 */
class PlayerWeapon1 : public Weapon
{
public:
    PlayerWeapon1()
    {
        name = "Shotgun";
        bulletSpeed = 20;
        bulletDamage = 7;
        reloadTicks = 20;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon1bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon2 extends Weapon
 */
class PlayerWeapon2 : public Weapon
{
public:
    PlayerWeapon2()
    {
        name = "Blaster";
        bulletSpeed = 20;
        bulletDamage = 13;
        reloadTicks = 100;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon2bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class PlayerWeapon3 extends Weapon
 */
class PlayerWeapon3 : public Weapon
{
public:
    PlayerWeapon3()
    {
        name = "Laser cannon";
        bulletSpeed = 50;
        bulletDamage = 100;
        reloadTicks = 60;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon3bullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon0 extends Weapon
 */
class EnemyWeapon0 : public Weapon
{
public:
    EnemyWeapon0()
    {
        bulletSpeed = 10;
        bulletDamage = 5;
        reloadTicks = 100;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon0ebullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon1 extends Weapon
 */
class EnemyWeapon1 : public Weapon
{
public:
    EnemyWeapon1()
    {
        bulletSpeed = 8;
        bulletDamage = 8;
        reloadTicks = 40;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon1ebullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class EnemyWeapon2 extends Weapon
 */
class EnemyWeapon2 : public Weapon
{
public:
    EnemyWeapon2()
    {
        bulletSpeed = 5;
        bulletDamage = 20;
        reloadTicks = 200;
        bulletTexture = getTexture("assets/projectnuma/textures/entity/weapon2ebullet.png");
    }

    void fire(Entity* owner, double degree) override;
};

/**
 * public class App
 */
class App
{
private:
    shared_ptr<Player> player;
    map<const char*, Weapon*> weapons;
    list <shared_ptr<Entity>> entities;
    list <shared_ptr<Entity>> environment;
    vector<UIComponent*> ui;

public:
    GameState state = STATE_STARTUP;
    bool pressedKey[1024]{};
    int killboardLevel[4]{};
    Level* currentLevel = null;

    void startup();

    void shutdown();

    void doSDLEvents();

    SDL_Renderer* getRenderer();

    Weapon* getWeapon(const char* name);

    Player* getPlayer();

    list <shared_ptr<Entity>> getEntities();

    vector<UIComponent*> getUI();

    UIComponent* addUIComponent(string name, SDL_Texture* texture, int x, int y);

    bool removeUIComponent(const UIComponent* c, bool freeTexture); // not tested

    bool removeUIComponent(const char* name, bool freeTexture);

    Entity* addEntity(Entity* e);

    void removeEntity(Entity* e, bool callOnDeath); //FIXME: crashes on call

    void render();

    void cleanupEntities();

    void mainLoop();

    void doStateMenu();

    void doStateLevels();

    void doStateGame(Level* level);

    void doStateHangar();

    void doStateSettings();

    void doStateAbout();
};

// CLASS DEFINITIONS END ===============================================================================================

// FUNCTION DEFINITIONS START ==========================================================================================

// Entity ==============================================================================================================

Entity::Entity(const EntityType type, const char* name, SDL_Texture* texture, Weapon* weapon, int maxHP, double speed,
               int side)
{
    this->type = type;
    this->name = string(name);
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
    if (beforeTick != null) beforeTick(this);

    // general tick operations
    move(dx, dy);
    if (weapon != null && reloadTicks > 0) reloadTicks--;
    if (type == ENTITY_TYPE_BULLET)
    {
        // this is bullet and hits something
        for (auto& e: app->getEntities())
        {
            if (!e->isDead && e->type != ENTITY_TYPE_BULLET && e->side != side && !e->isInvincible && CommonUtil.checkCollision(
                    e->x, e->y, e->width, e->height, x, y, width, height
            ))
            {
                this->hp = 0;
                e->hp -= ((Bullet*) this)->damage;
                playSound("assets/projectnuma/sounds/entity/hit.wav");
                break;
            }
        }
    }
    else if (type != ENTITY_TYPE_PLAYER && side != SIDE_PLAYER && CommonUtil.checkCollision(
            app->getPlayer()->x, app->getPlayer()->y, app->getPlayer()->width, app->getPlayer()->height,
            x, y, width, height
    ) && !app->getPlayer()->isDead && (!isInvincible && !app->getPlayer()->isInvincible))
    {
        // this is enemy and has collision with player
        int hp0 = hp, hp1 = app->getPlayer()->hp;
        hp -= hp1;
        app->getPlayer()->hp -= hp0;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s collides with player", name.c_str());
    }

    // tickAfter or not
    if (hp <= 0)
    {
        if (type != ENTITY_TYPE_BULLET)SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s dies", name.c_str());
        if (onDeath != null) onDeath(this);
        isDead = true;
    }
    else if (afterTick != null)
        afterTick(this);
}

// Player ==============================================================================================================

Player::Player()
{
    type = ENTITY_TYPE_PLAYER;
    isControllable = true;
    side = SIDE_PLAYER;
    maxHp = hp = 50;
    speed = 6;
    width = 40;
    height = 32;
    x = 100;
    y = WINDOW_HEIGHT / 2 - height / 2;
    texture = getTexture("assets/projectnuma/textures/entity/player.png");
    name = "Player";
    onSpawn = [](Entity* self) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player spawned");
    };
    beforeTick = [](Entity* self) {
        Player* p = (Player*) self;
        if (p->invincibleTicks >= 0) p->isInvincible = true;
        else p->isInvincible = false;
        if (p->isDead)
        {
            p->hp++;
            if (p->hp == 0)
            {
                if (p->life > 0)
                {
                    p->hp = p->maxHp;
                    p->isDead = false;
                    p->setLocation(100, WINDOW_HEIGHT / 2 - p->height / 2);
                    p->invincibleTicks = 150;
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player respawned");
                }
                else
                {
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Game over");
                    app->currentLevel->isFailed = true;
                }
            }
            return;
        }
        p->dx = p->dy = 0;
        // press W A S D to move, right-Shift to move faster, right-Ctrl to move slower
        if (app->pressedKey[SDL_SCANCODE_RSHIFT])
            p->speedModifier = 1.5;
        else if (app->pressedKey[SDL_SCANCODE_RCTRL])
            p->speedModifier = 0.7;
        if (app->pressedKey[SDL_SCANCODE_W])
            p->dy = -p->speed * p->speedModifier;
        if (app->pressedKey[SDL_SCANCODE_A])
            p->dx = -p->speed * p->speedModifier;
        if (app->pressedKey[SDL_SCANCODE_S])
            p->dy = p->speed * p->speedModifier;
        if (app->pressedKey[SDL_SCANCODE_D])
            p->dx = p->speed * p->speedModifier;
        // press SPACE to fire
        if (p->weapon != null && app->pressedKey[SDL_SCANCODE_SPACE] && p->reloadTicks == 0)
        {
            p->weapon->fire(p, 0);
            p->reloadTicks = p->weapon->reloadTicks;
        }
        // press right-Alt to switch weapon
        if (app->pressedKey[SDL_SCANCODE_RALT] && p->reloadTicks == 0)
        {
            for (int i = 0; i != p->currentWeapons.size(); i++)
                if (p->currentWeapons[i] == p->weapon)
                {
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Switch weapon");
                    if (i == p->currentWeapons.size() - 1)
                    {
                        p->weapon = p->currentWeapons[0];
                        p->reloadTicks = p->weapon->reloadTicks;
                    }
                    else
                    {
                        p->weapon = p->currentWeapons[i + 1];
                        p->reloadTicks = p->weapon->reloadTicks;
                    }
                    playSound("assets/projectnuma/sounds/ambient/weaponLoad.wav");
                    break;
                }
        }
    };
    afterTick = [](Entity* self) {
        self->setLocation(
                self->x + self->width > WINDOW_WIDTH ? WINDOW_WIDTH - self->width :
                self->x < 0 ? 0 :
                self->x,
                self->y + self->height > WINDOW_HEIGHT ? WINDOW_HEIGHT - self->height :
                self->y < 0 ? 0 :
                self->y
        );
        ((Player*) self)->speedModifier = 1.0;
        ((Player*) self)->invincibleTicks--;
    };
    onDeath = [](Entity* self) {
        playSound("assets/projectnuma/sounds/entity/playerDie.wav");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player died");
        self->hp = -150;
        ((Player*) self)->life--;
    };
    // load unlocked weapons from session file
    if (1 & session.unlockedWeapons)
        currentWeapons.emplace_back(app->getWeapon("PlayerWeapon0"));
    if (2 & session.unlockedWeapons)
        currentWeapons.emplace_back(app->getWeapon("PlayerWeapon1"));
    if (4 & session.unlockedWeapons)
        currentWeapons.emplace_back(app->getWeapon("PlayerWeapon2"));
    if (8 & session.unlockedWeapons)
        currentWeapons.emplace_back(app->getWeapon("PlayerWeapon3"));
    if (currentWeapons.empty())
    {
        session.unlockedWeapons = 1;
        weapon = app->getWeapon("PlayerWeapon0");
    }
    else weapon = currentWeapons[0];
}

// Enemy0 ==============================================================================================================

Enemy0::Enemy0() : Entity()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 5;
    reloadTicks = 10;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 2;
    dx = -speed + r.nextDouble(2.0) - 1.0;
    dy = r.nextDouble(1.0) - 0.5;
    width = 32;
    height = 24;
    texture = getTexture("assets/projectnuma/textures/entity/enemy0.png");
    weapon = app->getWeapon("EnemyWeapon0");
    name = "Turret";
    afterTick = [](Entity* self) {
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
        {
            playSound("assets/projectnuma/sounds/entity/hit.wav");
            session.credit++;
            app->killboardLevel[0]++;
        }
    };
}

// Enemy1 ==============================================================================================================

Enemy1::Enemy1() : Entity()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 30;
    reloadTicks = 20;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 1;
    dx = -speed + r.nextDouble(0.2) - 0.1;
    dy = r.nextDouble(0.2) - 0.1;
    width = 40;
    height = 32;
    texture = getTexture("assets/projectnuma/textures/entity/enemy1.png");
    weapon = app->getWeapon("EnemyWeapon1");
    name = "Enemy Fighter";
    afterTick = [](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            double deg = CommonUtil.getDegreeBeteween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    app->getPlayer()->x + app->getPlayer()->width / 2,
                    app->getPlayer()->y + app->getPlayer()->height / 2
            );
            if (deg >= 150 || deg <= -150)
            {
                self->weapon->fire(self, deg);
                self->reloadTicks = self->weapon->reloadTicks;
            }
        }
    };
    onDeath = [](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
        {
            playSound("assets/projectnuma/sounds/entity/enemyDie.wav");
            session.credit += 5;
            app->killboardLevel[1]++;
        }
    };
}

// Enemy2 ==============================================================================================================

Enemy2::Enemy2() : Entity()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 60;
    reloadTicks = 20;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 1;
    dx = -speed + r.nextDouble(0.2) - 0.1;
    dy = r.nextDouble(0.2) - 0.1;
    width = 60;
    height = 40;
    texture = getTexture("assets/projectnuma/textures/entity/enemy2.png");
    weapon = app->getWeapon("EnemyWeapon1");
    name = "Enemy armored fighter";
    afterTick = [](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            double deg = CommonUtil.getDegreeBeteween(
                    self->x + self->width / 2,
                    self->y + self->height / 2,
                    app->getPlayer()->x + app->getPlayer()->width / 2,
                    app->getPlayer()->y + app->getPlayer()->height / 2
            );
            if (deg >= 135 || deg <= -135)
            {
                self->weapon->fire(self, deg);
                self->reloadTicks = self->weapon->reloadTicks;
            }
        }
    };
    onDeath = [](Entity* self) {
        if (self->x >= 0 && self->y >= 0 && self->x + self->width <= WINDOW_WIDTH && self->y + self->height <= WINDOW_HEIGHT)
        {
            playSound("assets/projectnuma/sounds/entity/enemyDie.wav");
            session.credit += 10;
            app->killboardLevel[2]++;
        }
    };
}

// Enemy3 ==============================================================================================================

Enemy3::Enemy3() : Entity()
{
    type = ENTITY_TYPE_ENEMY;
    side = SIDE_ENEMY;
    maxHp = hp = 750;
    reloadTicks = 1000;
    x = WINDOW_WIDTH;
    y = 450;
    speed = 0.25;
    dx = -speed;
    width = 256;
    height = 160;
    texture = getTexture("assets/projectnuma/textures/entity/enemy3.png");
    weapon = app->getWeapon("EnemyWeapon2");
    name = "Enemy cruiser";
    onSpawn = [](Entity* self) {
        playSound("assets/projectnuma/sounds/ambient/warn.wav");
        self->isInvincible = true;
    };
    afterTick = [](Entity* self) {
        if (self->x + self->width <= 0 || self->y >= WINDOW_HEIGHT || self->y + self->height <= 0) self->hp = 0;
        else if (self->reloadTicks <= 0)
        {
            self->isInvincible = false;
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
        {
            playSound("assets/projectnuma/sounds/entity/bossDie.wav");
            session.credit += 50;
            app->killboardLevel[3]++;
        }
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
    beforeTick = [](Entity* self) {
        if (self->x + self->width <= 0 || self->y + self->height <= 0 || self->x >= WINDOW_WIDTH || self->y >= WINDOW_HEIGHT)
        {
            self->hp = 0;
            self->isDead = true;
        }
    };
}

// PlayerWeapon0 =======================================================================================================

void PlayerWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    degree += r.nextDouble(6.0) - 3.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(0, -b->height / 2);
    b->texture = bulletTexture;
    app->addEntity(b);
    playSound("assets/projectnuma/sounds/item/weapon0.wav");
}

// PlayerWeapon1 =======================================================================================================

void PlayerWeapon1::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    degree += r.nextDouble(6.0) - 3.0;
    for (int i = -2; i != 3; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 8, 8,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 3
        );
        b->move(0, -b->height / 2);
        b->texture = bulletTexture;
        app->addEntity(b);
    }
    playSound("assets/projectnuma/sounds/item/weapon1.wav");
}

// PlayerWeapon2 =======================================================================================================

void PlayerWeapon2::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    degree += r.nextDouble(10.0) - 5.0;
    for (int i = 0; i != 36; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 16, 16,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 10
        );
        b->move(-b->width / 2, -b->height / 2);
        b->texture = bulletTexture;
        app->addEntity(b);
    }
    playSound("assets/projectnuma/sounds/item/weapon2.wav");
}

// PlayerWeapon3 =======================================================================================================

void PlayerWeapon3::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s fire", name.c_str());
    Bullet* b = new Bullet(
            owner, bulletDamage, 200, 20,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(0, -b->height / 2);
    b->texture = bulletTexture;
    app->addEntity(b);
    playSound("assets/projectnuma/sounds/item/weapon3.wav");
}

// EnemyWeapon0 ========================================================================================================

void EnemyWeapon0::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy weapon 0 fire");
    degree += r.nextDouble(2.0) - 1.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(-b->width / 2, -b->height / 2);
    b->texture = bulletTexture;
    app->addEntity(b);
    playSound("assets/projectnuma/sounds/item/weapon0e.wav");
}

// EnemyWeapon1 ========================================================================================================

void EnemyWeapon1::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy weapon 1 fire");
    degree += r.nextDouble(2.0) - 1.0;
    Bullet* b = new Bullet(
            owner, bulletDamage, 8, 8,
            owner->x + owner->width / 2, owner->y + owner->height / 2,
            bulletSpeed, degree
    );
    b->move(-b->width / 2, -b->height / 2);
    b->texture = bulletTexture;
    app->addEntity(b);
    playSound("assets/projectnuma/sounds/item/weapon1e.wav");
}

// EnemyWeapon2 ========================================================================================================

void EnemyWeapon2::fire(Entity* owner, double degree)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Enemy weapon 2 fire");
    degree += r.nextDouble(6.0) - 3.0;
    for (int i = -7; i != 8; i++)
    {
        Bullet* b = new Bullet(
                owner, bulletDamage, 16, 16,
                owner->x + owner->width / 2, owner->y + owner->height / 2,
                bulletSpeed, degree - i * 3
        );
        b->hp = 150;
        b->move(-b->width / 2, -b->height / 2);
        b->texture = bulletTexture;
        b->afterTick = [](Entity* self) {
            self->hp--;
        };
        app->addEntity(b);
    }
    playSound("assets/projectnuma/sounds/item/weapon2e.wav");
}

// App =================================================================================================================

void App::startup()
{
    app = this;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is coming up. Please wait");
    RenderManager.init();
    SoundManager.init();
    // initialize weapons
    weapons.emplace("PlayerWeapon0", new PlayerWeapon0());
    weapons.emplace("PlayerWeapon1", new PlayerWeapon1());
    weapons.emplace("PlayerWeapon2", new PlayerWeapon2());
    weapons.emplace("PlayerWeapon3", new PlayerWeapon3());
    weapons.emplace("EnemyWeapon0", new EnemyWeapon0());
    weapons.emplace("EnemyWeapon1", new EnemyWeapon1());
    weapons.emplace("EnemyWeapon2", new EnemyWeapon2());
    // load session
    session.load();
    // initialize player
    player = make_shared<Player>(Player());
    addEntity(player.get());
    player->maxHp = session.hp;
    player->hp = session.hp;
    // completed
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is ready");
    state = STATE_MENU;
    mainLoop();
}

void App::shutdown()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is coming down. Please wait");
    // shutdown sdl
    SoundManager.shutdown();
    RenderManager.shutdown();
    // save session
    session.hp = player->maxHp;
    session.save();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shutdown completed");
}

void App::doSDLEvents()
{
    // tick SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "FORCE SHUTDOWN! (SDL_QUIT signal received)");
                shutdown();
                exit(0);
            }
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
}

SDL_Renderer* App::getRenderer()
{
    return renderer;
}

Weapon* App::getWeapon(const char* name)
{
    Weapon* w = weapons[name];
    if (w == null) throw runtime_error(string("Weapon not found: ").append(name));
    return w;
}

Player* App::getPlayer()
{
    return player.get();
}

list <shared_ptr<Entity>> App::getEntities()
{
    return entities;
}

vector<UIComponent*> App::getUI()
{
    return ui;
}

UIComponent* App::addUIComponent(string name, SDL_Texture* texture, int x, int y)
{
    ui.emplace_back(new UIComponent(name, texture, x, y));
    return ui.at(ui.size() - 1);
}

bool App::removeUIComponent(const UIComponent* c, bool freeTexture)
{
    for (int i = 0; i != ui.size(); i++)
        if (ui[i] == c)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Remove UI %s", ui[i]->name.c_str());
            if (freeTexture) ui[i]->setTexture(null, true);
            ui.erase(ui.begin() + i);
            return true;
        }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "UI %s not found", c->name.c_str());
    return false;
}

bool App::removeUIComponent(const char* name, bool freeTexture)
{
    for (int i = 0; i != ui.size(); i++)
        if (ui.at(i)->name == name)
        {
            if (freeTexture) ui[i]->setTexture(null, true);
            ui.erase(ui.begin() + i);
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Removed UI %s", name);
            return true;
        }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "UI %s not found", name);
    return false;
}

Entity* App::addEntity(Entity* e)
{
    if (e->type != ENTITY_TYPE_BULLET)SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Add entity %s", e->name.c_str());
    entities.emplace_back(e);
    if (e->onSpawn != null) e->onSpawn(e);
    return e;
}

void App::removeEntity(Entity* e, bool callOnDeath)
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

void App::render()
{
    // refresh screen
    SDL_SetRenderDrawColor(renderer, 32, 32, 64, 255);
    SDL_RenderClear(renderer);
    // place entities
    for (const auto& e: entities)
    {
        if (!e->isDead) placeTexture(e->texture, e->x, e->y, e->width, e->height);
    }
    // place ui components
    for (const auto& a: ui)
        placeTexture(a->texture, a->x, a->y);
    SDL_RenderPresent(renderer);
}

void App::cleanupEntities()
{
    entities.remove_if(
            [](const shared_ptr<Entity>& e) { return e->isDead && e->type != ENTITY_TYPE_PLAYER; }
    );
}

void App::mainLoop()
{
    for (;;)
    {
        switch (state)
        {
            case STATE_STARTUP:
            {
                throw std::logic_error("App.state is not supposed to be STATE_STARTUP in mainLoop()");
            }
            case STATE_MENU:
            {
                doStateMenu();
                break;
            }
            case STATE_LEVELS:
            {
                doStateLevels();
            }
            case STATE_GAME:
            {
                Level defaultLevel = LevelUtil.loadLevel("assets/projectnuma/levels/infinite.level");
                doStateGame(&defaultLevel);
                break;
            }
            case STATE_HANGAR:
            {
                doStateHangar();
                break;
            }
            case STATE_SETTINGS:
            {
                doStateSettings();
                break;
            }
            case STATE_ABOUT:
            {
                doStateAbout();
                break;
            }
            case STATE_SHUTDOWN:
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shutdown sequence initiated");
                shutdown();
                goto exitMainLoop;
            }
        }
    }
    exitMainLoop:;
}

void App::doStateMenu()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show main menu");
    getPlayer()->setLocation(WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Texture* logo = getTexture("assets/projectnuma/textures/gui/title.png");
    SDL_Texture* startGame = RenderManager.getText("Press [SPACE] to start the game", 255, 255, 255, 255, FONT_SIZE_L);
    SDL_Texture* quitGame = RenderManager.getText("Press [ESC] to quit the game", 255, 255, 255, 255, FONT_SIZE_L);
    addUIComponent("logo", logo, WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(logo) / 2, 200);
    addUIComponent("start", startGame, WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(startGame) / 2, 500);
    addUIComponent("quit", quitGame, WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(quitGame) / 2, 550);
    setBGM("assets/projectnuma/sounds/music/menu/0.ogg");
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_ESCAPE]) // quit game
        {
            state = STATE_SHUTDOWN;
            return;
        }
        if (pressedKey[SDL_SCANCODE_SPACE]) // start main game
        {
            removeUIComponent("logo", false);
            removeUIComponent("start", true);
            removeUIComponent("quit", true);
            pressedKey[SDL_SCANCODE_SPACE] = false;
            state = STATE_GAME;
            return;
        }
        render();
        SDL_Delay(16);
    }
}

void App::doStateLevels() //TODO
{
    ;
}

void App::doStateGame(Level* level)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Entering main game\n\tUsing level: %s", level->name.c_str());
    currentLevel = level;
    // initialize ui
    addUIComponent("version", RenderManager.getText(VERSION, 127, 255, 255, 255, FONT_SIZE_M), 10, 10);
    addUIComponent("weapon indicator", RenderManager.getText("A", 255, 255, 255, 255, FONT_SIZE_M), 10, 40);
    addUIComponent("hp indicator", RenderManager.getText("A", 255, 255, 255, 255, FONT_SIZE_M), 10, 70);
    addUIComponent("credit indicator", RenderManager.getText("A", 255, 255, 255, 255, FONT_SIZE_M), 10, 100);
    addUIComponent("life indicator", RenderManager.getText("A", 255, 255, 255, 255, FONT_SIZE_M), 10, 130);
    getPlayer()->life = level->playerLife;
    getPlayer()->hp = getPlayer()->maxHp;
    getPlayer()->isDead = false;
    getPlayer()->setLocation(100, WINDOW_HEIGHT / 2 - getPlayer()->height / 2);
    setBGM("assets/projectnuma/sounds/music/game/0.ogg");
    for (uint64_t tick = 0;; tick++)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_ESCAPE])
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Leaving main game");
            goto toMainMenu;
        }
        // check level goal
        if (
                killboardLevel[0] >= level->killGoal[0] ||
                killboardLevel[1] >= level->killGoal[1] ||
                killboardLevel[2] >= level->killGoal[2] ||
                killboardLevel[3] >= level->killGoal[3]
                )
        {
            level->isPassed = true;
        }

        if (currentLevel->isPassed)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Level passed: %s", level->name.c_str());
            if (level->id != 0)session.unlockedLevels = MAX(level->id, session.unlockedLevels);
            goto toMainMenu;
        }
        if (currentLevel->isFailed)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Level failed: %s", level->name.c_str());
            goto toMainMenu;
        }

        // randomly add enemies
        if (r.nextDouble(1) <= level->spawnRateBase)
            switch (r.nextInt(4))
            {
                case 0:
                {
                    if (r.nextDouble(1) <= level->spawnRateEnemy0)
                        addEntity((new Enemy0())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 24)));
                    break;
                }
                case 1:
                {
                    if (r.nextDouble(1) <= level->spawnRateEnemy1)
                        addEntity((new Enemy1())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 32)));
                    break;
                }
                case 2:
                {
                    if (r.nextDouble(1) <= level->spawnRateEnemy2)
                        addEntity((new Enemy2())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 40)));
                    break;
                }
                case 3:
                {
                    if (r.nextDouble(1) <= level->spawnRateEnemy3)
                        addEntity((new Enemy3())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 160)));
                    break;
                }
            }
        // tick entities
        for (const shared_ptr<Entity>& e: entities)
        {
            if (!e->isDead)
                e->tick();
            else if (e->type == ENTITY_TYPE_PLAYER && e->isDead) e->beforeTick(e.get());
        }
        // tick UI
        for (auto it: ui)
            if (it->name == "weapon indicator")
                it->setTexture(
                        RenderManager.getText(string("Weapon: ").append(player->weapon->name).c_str(),
                                              255, 255, 255, 127, FONT_SIZE_M), true);
            else if (it->name == "hp indicator")
                it->setTexture(
                        RenderManager.getText(string("HP: ").append(to_string(player->hp)).c_str(),
                                              255, 255, 255, 127, FONT_SIZE_M), true);
            else if (it->name == "credit indicator")
                it->setTexture(
                        RenderManager.getText(string("Credit: ").append(to_string(session.credit)).c_str(),
                                              255, 255, 255, 127, FONT_SIZE_M), true);
            else if (it->name == "life indicator")
                it->setTexture(
                        RenderManager.getText(string("Life: ").append(to_string(getPlayer()->life)).c_str(),
                                              255, player->life == 0 ? 0 : 255, player->life == 0 ? 0 : 255, 127,
                                              FONT_SIZE_M), true);
        render();
        // clean up
        if (tick % 100 == 0)
            cleanupEntities();
        SDL_Delay(16);
    }
    toMainMenu:
    // remove ui
    removeUIComponent("version", true);
    removeUIComponent("weapon indicator", true);
    removeUIComponent("hp indicator", true);
    removeUIComponent("credit indicator", true);
    removeUIComponent("life indicator", true);
    for (const auto& a: entities) // remove all entities
        if (a->type != ENTITY_TYPE_PLAYER)
            a->isDead = true;
    cleanupEntities();
    for (int i = 0; i != 4; i++) // move kill counts to total
    {
        session.killboardTotal[i] += killboardLevel[i];
        killboardLevel[i] = 0;
    }
    memset(pressedKey, false, sizeof pressedKey); // reset keyboard
    currentLevel = null;
    state = STATE_MENU;
}

void App::doStateHangar() //TODO
{
    ;
}

void App::doStateSettings() //TODO
{
    ;
}

void App::doStateAbout() //TODO
{
    ;
}

// FUNCTION DEFINITIONS END ============================================================================================

#endif //PROJECTNUMA_PROJECTNUMA_H
