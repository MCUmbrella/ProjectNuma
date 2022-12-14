//
// Created by mcumbrella on 22-11-8.
//

#ifndef PROJECTNUMA_PROJECTNUMA_H
#define PROJECTNUMA_PROJECTNUMA_H

#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <list>
#include <stdexcept>
#include <string>
#include "GeneralDefinitions.h"
#include "CommonUtil.h"
#include "Random.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "Session.h"
#include "UIComponent.h"
#include "LevelUtil.h"
#include "Entity.h"
#include "Player.h"
#include "Enemies.h"
#include "Bullet.h"
#include "Weapon.h"
#include "PlayerWeapons.h"
#include "EnemyWeapons.h"
#include "App.h"

// import static
#define playSound SoundManager.playSound
#define setBGM SoundManager.setBGM
#define getTexture RenderManager.getTexture
#define placeTexture RenderManager.placeTexture
#define textToTexture RenderManager.textToTexture

static App* app;
static Session session;
static Random r;

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
        for (auto& e : app->getEntities())
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
        if (p->invincibleTicks > 0) p->isInvincible = true;
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
        if (app->pressedKey[SDL_SCANCODE_RALT])
        {
            for (unsigned long i = 0; i != p->currentWeapons.size(); i++)
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
            app->pressedKey[SDL_SCANCODE_RALT] = false;
        }
    };
    afterTick = [](Entity* self) {
        Player* p = (Player*) self;
        p->setLocation(
                p->x + p->width > WINDOW_WIDTH ? WINDOW_WIDTH - p->width :
                p->x < 0 ? 0 :
                p->x,
                p->y + p->height > WINDOW_HEIGHT ? WINDOW_HEIGHT - p->height :
                p->y < 0 ? 0 :
                p->y
        );
        p->speedModifier = 1.0;
        p->invincibleTicks -= p->invincibleTicks > 0 ? 1 : 0;
    };
    onDeath = [](Entity* self) {
        playSound("assets/projectnuma/sounds/entity/playerDie.wav");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player died");
        self->hp = -100;
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
    onSpawn = [](Entity* self) {
        app->enemyCounter[0]++;
    };
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
            app->killboardLevel[0]++;
        }
        app->enemyCounter[0]--;
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
    onSpawn = [](Entity* self) {
        app->enemyCounter[1]++;
    };
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
        app->enemyCounter[1]--;
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
    onSpawn = [](Entity* self) {
        app->enemyCounter[2]++;
    };
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
        app->enemyCounter[2]--;
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
        app->enemyCounter[3]++;
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
        app->enemyCounter[3]--;
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
    if (flDebug) SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
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

void App::resetKeyState()
{
    memset(pressedKey, false, sizeof pressedKey); // reset keyboard
}

Weapon* App::getWeapon(const char* name)
{
    Weapon* w = weapons[name];
    if (w == null) throw runtime_error(string("Weapon not found: ") + name);
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

UIComponent* App::addUIComponent(string name, SDL_Texture* texture, int x, int y, bool freeTextureOnDestruct)
{
    ui.emplace_back(new UIComponent(name, texture, x, y, freeTextureOnDestruct));
    return ui.at(ui.size() - 1);
}

bool App::removeUIComponent(const UIComponent* c)
{
    for (unsigned long i = 0; i != ui.size(); i++)
        if (ui[i] == c)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Remove UI %s", ui[i]->name.c_str());
            delete ui[i];
            ui.erase(ui.begin() + i);
            return true;
        }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "UI %s not found", c->name.c_str());
    return false;
}

bool App::removeUIComponent(const char* name)
{
    for (unsigned long i = 0; i != ui.size(); i++)
        if (ui[i]->name == name)
        {
            delete ui[i];
            ui.erase(ui.begin() + i);
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Removed UI %s", name);
            return true;
        }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "UI %s not found", name);
    return false;
}

void App::clearAllUI()
{
    for (UIComponent* u : ui)
        delete u;
    ui.clear();
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
    for (const shared_ptr<Entity>& ee : entities)
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
    for (const auto& e : entities)
    {
        if (!e->isDead) placeTexture(e->texture, e->x, e->y, e->width, e->height);
    }
    // place ui components
    for (const auto& a : ui)
        placeTexture(a->texture, a->x, a->y);
    SDL_RenderPresent(renderer);
}

bool App::showPrompt(const char* msg, bool showHints)
{
    const char* item[] = {"msgShadow", "msg", "yesShadow", "yes", "noShadow", "no"};
    SDL_Texture* textTexture = textToTexture(msg, 255, 255, 0, 255, FONT_SIZE_XL);
    SDL_Texture* yes = textToTexture("[SPACE] Proceed", 200, 200, 200, 255, FONT_SIZE_L);
    SDL_Texture* no = textToTexture("[ESC] Cancel", 200, 200, 200, 255, FONT_SIZE_L);
    SDL_Texture* textTextureShadow = textToTexture(msg, 0, 0, 0, 127, FONT_SIZE_XL);
    SDL_Texture* yesShadow = textToTexture("[SPACE] Proceed", 0, 0, 0, 127, FONT_SIZE_L);
    SDL_Texture* noShadow = textToTexture("[ESC] Cancel", 0, 0, 0, 127, FONT_SIZE_L);
    addUIComponent(item[0], textTextureShadow,
                   WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(textTexture) / 2 + 3, 403, true);
    addUIComponent(item[1], textTexture,
                   WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(textTexture) / 2, 400, true);
    addUIComponent(item[2], yesShadow,
                   WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(yes) / 2 + 3, showHints ? 463 : 1000, true);
    addUIComponent(item[3], yes,
                   WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(yes) / 2, showHints ? 460 : 1000, true);
    addUIComponent(item[4], noShadow,
                   WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(no) / 2 + 3, showHints ? 513 : 1000, true);
    addUIComponent(item[5], no,
                   WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(no) / 2, showHints ? 510 : 1000, true);
    playSound("assets/projectnuma/sounds/misc/msg.wav");
    //TODO: darken the screen when displaying prompt
    for (;;)
    {
        resetKeyState();
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_RETURN] || pressedKey[SDL_SCANCODE_SPACE])
        {
            for (const char* s : item)
                removeUIComponent(s);
            resetKeyState();
            return true;
        }
        else if (pressedKey[SDL_SCANCODE_ESCAPE] || pressedKey[SDL_SCANCODE_BACKSPACE])
        {
            for (const char* s : item)
                removeUIComponent(s);
            resetKeyState();
            return false;
        }
        render();
        SDL_Delay(50);
    }
}

int App::showOptions(initializer_list<string> args, int x, int y, unsigned char alignMode, FontSize fontSize)
{
    if (args.size() == 0) return 0;
    alignMode = alignMode % 3;
    vector<SDL_Texture*> optionText;
    vector<SDL_Texture*> optionTextS;
    vector<UIComponent*> optionUI;
    for (const string& s : args) // generate text textures
    {
        optionText.emplace_back(textToTexture(s.c_str(), 255, 255, 255, 255, fontSize));
        optionTextS.emplace_back(textToTexture(s.c_str(), 0, 255, 0, 255, fontSize));
    }
    for (int i = 0; i != args.size(); i++) // generate text UIs
        optionUI.emplace_back(addUIComponent(
                string("option").append(to_string(i)), optionText[i],
                alignMode == 2 ? x - RenderManager.getTextureWidth(optionText[i]) :
                alignMode == 1 ? x - RenderManager.getTextureWidth(optionText[i]) / 2
                               : x,
                y + i * (fontSize == FONT_SIZE_XL ? 60 :
                         fontSize == FONT_SIZE_L ? 50 :
                         fontSize == FONT_SIZE_M ? 40 :
                         fontSize == FONT_SIZE_S ? 30 :
                         20),
                false)
        );
    function<void()> removeUI = [&]() {
        for (int i = 0; i != args.size(); i++)
            removeUIComponent(string("option").append(to_string(i)).c_str());
        for (auto t : optionText) SDL_DestroyTexture(t);
        for (auto t : optionTextS) SDL_DestroyTexture(t);
        resetKeyState();
    };
    playSound("assets/projectnuma/sounds/misc/msg.wav");
    int selection = 0;
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_S] || pressedKey[SDL_SCANCODE_DOWN])
        {
            if (selection == args.size() - 1) selection = 0;
            else selection++;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_W] || pressedKey[SDL_SCANCODE_UP])
        {
            if (selection == 0) selection = args.size() - 1;
            else selection--;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_SPACE] || pressedKey[SDL_SCANCODE_RETURN])
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Selected %d (%s)", selection, optionUI[selection]->name.c_str());
            removeUI();
            return selection;
        }

        // highlight the selected option
        for (int i = 0; i != args.size(); i++)
            optionUI[i]->setTexture(optionText[i], false);
        optionUI[selection]->setTexture(optionTextS[selection], false);

        render();
        SDL_Delay(50);
    }
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
                break;
            }
            case STATE_GAME:
            {
                Level defaultLevel = LevelUtil.loadLevel("assets/projectnuma/levels/infiniteNormal.level");
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
                return;
            }
        }
    }
}

void App::doStateMenu()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show main menu");
    if (bgm != SoundManager.getMusic("assets/projectnuma/sounds/music/menu/0.ogg"))
        setBGM("assets/projectnuma/sounds/music/menu/0.ogg");
    getPlayer()->setLocation(WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_Texture* logo = getTexture("assets/projectnuma/textures/gui/title.png");

    SDL_Texture* item[] = {
            textToTexture("START", 255, 255, 255, 255, FONT_SIZE_XL),
            textToTexture("QUIT", 255, 255, 255, 255, FONT_SIZE_XL),
            textToTexture("HANGAR", 255, 255, 255, 255, FONT_SIZE_XL),
            textToTexture("SETTINGS", 255, 255, 255, 255, FONT_SIZE_XL),
            textToTexture("ABOUT", 255, 255, 255, 255, FONT_SIZE_XL),
    };

    SDL_Texture* itemSelected[] = {
            textToTexture("START", 0, 255, 0, 255, FONT_SIZE_XL),
            textToTexture("QUIT", 0, 255, 0, 255, FONT_SIZE_XL),
            textToTexture("HANGAR", 0, 255, 0, 255, FONT_SIZE_XL),
            textToTexture("SETTINGS", 0, 255, 0, 255, FONT_SIZE_XL),
            textToTexture("ABOUT", 0, 255, 0, 255, FONT_SIZE_XL),
    };

    addUIComponent("logo", logo, WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(logo) / 2, 150, false);

    UIComponent* startUI = addUIComponent("start", item[0],
                                          WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(item[0]) / 2, 450, true);
    UIComponent* quitUI = addUIComponent("quit", item[1],
                                         WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(item[1]) / 2, 510, true);
    UIComponent* hangarUI = addUIComponent("hangar", item[2],
                                           WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(item[2]) / 2, 570, true);
    UIComponent* settingsUI = addUIComponent("settings", item[3],
                                             WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(item[3]) / 2, 630, true);
    UIComponent* aboutUI = addUIComponent("about", item[4],
                                          WINDOW_WIDTH / 2 - RenderManager.getTextureWidth(item[4]) / 2, 690, true);

    function<void()> removeUI = [&]() {
        clearAllUI();
        for (SDL_Texture* t : item)
            SDL_DestroyTexture(t);
        for (SDL_Texture* t : itemSelected)
            SDL_DestroyTexture(t);
        resetKeyState();
    };

    int selection = 0;
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_S] || pressedKey[SDL_SCANCODE_DOWN])
        {
            if (selection == 4) selection = 0;
            else selection++;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_W] || pressedKey[SDL_SCANCODE_UP])
        {
            if (selection == 0) selection = 4;
            else selection--;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_SPACE] || pressedKey[SDL_SCANCODE_RETURN])
        {
            switch (selection)
            {
                case 0: // start game
                {
                    removeUI();
                    state = STATE_GAME;
                    return;
                }
                case 1: // quit
                {
                    state = STATE_SHUTDOWN;
                    return;
                }
                case 2: // hangar
                {
                    removeUI();
                    state = STATE_HANGAR;
                    return;
                }
                case 3: // settings
                {
                    showPrompt("This feature has not been implemented yet", true);
                    break;
                }
                case 4: // about
                {
                    removeUI();
                    state = STATE_ABOUT;
                    return;
                }
            }
        }

        // highlight the selected option
        if (selection == 0) startUI->setTexture(itemSelected[0], false);
        else startUI->setTexture(item[0], false);
        if (selection == 1) quitUI->setTexture(itemSelected[1], false);
        else quitUI->setTexture(item[1], false);
        if (selection == 2) hangarUI->setTexture(itemSelected[2], false);
        else hangarUI->setTexture(item[2], false);
        if (selection == 3) settingsUI->setTexture(itemSelected[3], false);
        else settingsUI->setTexture(item[3], false);
        if (selection == 4) aboutUI->setTexture(itemSelected[4], false);
        else aboutUI->setTexture(item[4], false);

        render();
        SDL_Delay(50);
    }
}

void App::doStateLevels()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show levels page");
}

void App::doStateGame(Level* level)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Entering main game\n\tUsing level: %s", level->name.c_str());
    currentLevel = level;
    // initialize ui
    addUIComponent("level indicator", textToTexture(level->name.c_str(), 255, 255, 0, 200, FONT_SIZE_S), 10, 10, true);
    addUIComponent("weapon indicator", textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 40, true);
    addUIComponent("hp indicator", textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 70, true);
    addUIComponent("credit indicator", textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 100, true);
    addUIComponent("life indicator", textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 130, true);
    getPlayer()->life = level->playerLife;
    getPlayer()->hp = getPlayer()->maxHp;
    getPlayer()->isDead = false;
    getPlayer()->setLocation(100, WINDOW_HEIGHT / 2 - getPlayer()->height / 2);
    setBGM("assets/projectnuma/sounds/music/game/0.ogg");
    for (unsigned long tick = 0, time0 = 0, time1 = 0; (time0 = CommonUtil.currentTimeNanos()); tick++)
    {
        assert(enemyCounter[0] >= 0 && enemyCounter[1] >= 0 && enemyCounter[2] >= 0 && enemyCounter[3] >= 0);
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_ESCAPE])
        {
            if (showPrompt("Return to the main menu?", true))
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Leaving main game");
                goto toMainMenu;
            }
            time0 = CommonUtil.currentTimeNanos();
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
            showPrompt("LEVEL PASSED!", false);
            goto toMainMenu;
        }
        if (currentLevel->isFailed)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Level failed: %s", level->name.c_str());
            showPrompt("GAME OVER!", false);
            goto toMainMenu;
        }

        // randomly add enemies
        if (r.nextDouble(1) <= level->randomSpawnSpeed)
            switch (r.nextInt(4))
            {
                case 0:
                {
                    if (enemyCounter[0] < level->maxEnemiesInScreen[0] && r.nextDouble(1) <= level->enemySpawnRate[0])
                        addEntity((new Enemy0())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 24)));
                    break;
                }
                case 1:
                {
                    if (enemyCounter[1] < level->maxEnemiesInScreen[1] && r.nextDouble(1) <= level->enemySpawnRate[1])
                        addEntity((new Enemy1())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 32)));
                    break;
                }
                case 2:
                {
                    if (enemyCounter[2] < level->maxEnemiesInScreen[2] && r.nextDouble(1) <= level->enemySpawnRate[2])
                        addEntity((new Enemy2())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 40)));
                    break;
                }
                case 3:
                {
                    if (enemyCounter[3] < level->maxEnemiesInScreen[3] && r.nextDouble(1) <= level->enemySpawnRate[3])
                        addEntity((new Enemy3())->setLocation(WINDOW_WIDTH, r.nextInt(WINDOW_HEIGHT - 160)));
                    break;
                }
            }
        // tick entities
        for (const shared_ptr<Entity>& e : entities)
        {
            if (!e->isDead)
                e->tick();
            else if (e->type == ENTITY_TYPE_PLAYER && e->isDead) e->beforeTick(e.get());
        }
        // tick UI
        for (const auto& it : ui)
            if (it->name == "weapon indicator")
                it->setTexture(
                        textToTexture(string("Weapon: ").append(player->weapon->name).c_str(),
                                      255, 255, 255, 200, FONT_SIZE_S), true);
            else if (it->name == "hp indicator")
                it->setTexture(
                        textToTexture(string("HP: ").append(to_string(player->hp)).c_str(),
                                      255 * (150 - player->invincibleTicks) / 150, player->hp > 0 ? 255 : 0,
                                      player->hp > 0 ? 255 : 0, 200, FONT_SIZE_S), true);
            else if (it->name == "credit indicator")
                it->setTexture(
                        textToTexture(string("Credit: ").append(to_string(session.credit)).c_str(),
                                      255, 255, 255, 200, FONT_SIZE_S), true);
            else if (it->name == "life indicator")
                it->setTexture(
                        textToTexture(string("Life: ").append(to_string(getPlayer()->life)).c_str(),
                                      255, player->life == 0 ? 0 : 255, player->life == 0 ? 0 : 255, 200,
                                      FONT_SIZE_S), true);
        render();
        // clean up
        if (tick % 10 == 0)
            cleanupEntities();
        time1 = CommonUtil.currentTimeNanos();
        if (time1 - time0 > 16000000LU)
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Can't keep up! Is the game overloaded or the system time has changed?\n\tA single tick took %lu ns", time1 - time0);
        SDL_Delay(16 - MIN(16LU, (time1 - time0) / 1000000LU));
    }
    toMainMenu:
    // remove ui
    clearAllUI();
    for (const auto& a : entities) // remove all entities
        if (a->type != ENTITY_TYPE_PLAYER)
            a->isDead = true;
    cleanupEntities();
    for (int i = 0; i != 4; i++) // move kill counts to total
    {
        session.killboardTotal[i] += killboardLevel[i];
        killboardLevel[i] = 0;
    }
    memset(enemyCounter, 0, sizeof(enemyCounter));
    assert(entities.size() == 1);
    resetKeyState();
    currentLevel = null;
    state = STATE_MENU;
}

void App::doStateHangar()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show hangar page");
    SDL_Texture* item[] = {
            textToTexture("Shotgun (600CR)", 255, 255, 255, 255, FONT_SIZE_M),
            textToTexture("Blaster (2000CR)", 255, 255, 255, 255, FONT_SIZE_M),
            textToTexture("Laser cannon (2000CR)", 255, 255, 255, 255, FONT_SIZE_M),
            textToTexture("+10 max HP (1880CR)", 255, 255, 255, 255, FONT_SIZE_M),
    };
    SDL_Texture* itemSelected[] = {
            textToTexture("Shotgun (600CR)", 0, 255, 0, 255, FONT_SIZE_M),
            textToTexture("Blaster (2000CR)", 0, 255, 0, 255, FONT_SIZE_M),
            textToTexture("Laser cannon (2000CR)", 0, 255, 0, 255, FONT_SIZE_M),
            textToTexture("+10 max HP (1880CR)", 0, 255, 0, 255, FONT_SIZE_M),
    };

    addUIComponent("hangar", textToTexture("Hangar", 255, 255, 0, 255, FONT_SIZE_XL), 20, 20, true);
    addUIComponent("back", textToTexture("[Esc] Go back", 255, 255, 255, 255, FONT_SIZE_L), 20, 850, true);
    UIComponent* w1ui = addUIComponent("w1", item[0], 50, 80, false);
    UIComponent* w2ui = addUIComponent("w2", item[1], 50, 120, false);
    UIComponent* w3ui = addUIComponent("w3", item[2], 50, 160, false);
    UIComponent* hpui = addUIComponent("hp", item[3], 50, 200, false);
    UIComponent* cr = addUIComponent("credit", textToTexture("A", 255, 255, 255, 255, FONT_SIZE_L), 20, 800, true);

    int selection = 0;
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_ESCAPE])
        {
            clearAllUI();
            for (SDL_Texture* t : item)
                SDL_DestroyTexture(t);
            for (SDL_Texture* t : itemSelected)
                SDL_DestroyTexture(t);
            resetKeyState();
            state = STATE_MENU;
            return;
        }
        else if (pressedKey[SDL_SCANCODE_S] || pressedKey[SDL_SCANCODE_DOWN])
        {
            if (selection == 3) selection = 0;
            else selection++;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_W] || pressedKey[SDL_SCANCODE_UP])
        {
            if (selection == 0) selection = 3;
            else selection--;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_SPACE] || pressedKey[SDL_SCANCODE_RETURN])
        {
            switch (selection)
            {
                case 0: // buy Weapon1 with 600cr
                {
                    if (showPrompt("Purchase this weapon?", true))
                    {
                        if (session.unlockedWeapons & 2) // check if this hasent been purchased before
                        {
                            showPrompt("You have already purchased this!", false);
                        }
                        else
                        {
                            if (session.credit >= 600) // check if your money is enough
                            {
                                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase weapon 1");
                                session.unlockedWeapons = session.unlockedWeapons | 2;
                                session.credit -= 600;
                                player->currentWeapons.emplace_back(getWeapon("PlayerWeapon1"));
                                showPrompt("Purchase success!", false);
                            }
                            else showPrompt("You don't have enough credits!", false);

                        }
                    }
                    resetKeyState();
                    break;
                }
                case 1: // buy Weapon2 with 2000cr
                {
                    if (showPrompt("Purchase this weapon?", true))
                    {
                        if (session.unlockedWeapons & 4)
                        {
                            showPrompt("You have already purchased this!", false);
                        }
                        else
                        {
                            if (session.credit >= 2000)
                            {
                                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase weapon 2");
                                session.unlockedWeapons = session.unlockedWeapons | 4;
                                session.credit -= 2000;
                                player->currentWeapons.emplace_back(getWeapon("PlayerWeapon2"));
                                showPrompt("Purchase success!", false);
                            }
                            else showPrompt("You don't have enough credits!", false);

                        }

                    }
                    resetKeyState();
                    break;
                }
                case 2: // buy Weapon3 with 2000cr
                {
                    if (showPrompt("Purchase this weapon?", true))
                    {
                        if (session.unlockedWeapons & 8)
                        {
                            showPrompt("You have already purchased this!", false);
                        }
                        else
                        {
                            if (session.credit >= 2000)
                            {
                                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase weapon 3");
                                session.unlockedWeapons = session.unlockedWeapons | 8;
                                session.credit -= 2000;
                                player->currentWeapons.emplace_back(getWeapon("PlayerWeapon3"));
                                showPrompt("Purchase success!", false);
                            }
                            else showPrompt("You don't have enough credits!", false);
                        }
                    }
                    resetKeyState();
                    break;
                }
                case 3: // buy 10 max hp with 1880cr
                {
                    if (showPrompt("Purchase this upgrade?", true))
                    {
                        if (session.credit >= 1880)
                        {
                            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase 10 max HP");
                            session.hp += 10;
                            player->maxHp += 10;
                            session.credit -= 1880;
                            showPrompt("Purchase success!", false);
                        }
                        else showPrompt("You don't have enough credits!", false);
                    }
                    resetKeyState();
                    break;
                }
            }
        }

        // highlight the selected option
        if (selection == 0) w1ui->setTexture(itemSelected[0], false);
        else w1ui->setTexture(item[0], false);
        if (selection == 1) w2ui->setTexture(itemSelected[1], false);
        else w2ui->setTexture(item[1], false);
        if (selection == 2) w3ui->setTexture(itemSelected[2], false);
        else w3ui->setTexture(item[2], false);
        if (selection == 3) hpui->setTexture(itemSelected[3], false);
        else hpui->setTexture(item[3], false);

        cr->setTexture(
                textToTexture(string("Credit: ").append(to_string(session.credit)).c_str(),
                              255, 255, 255, 255, FONT_SIZE_L), true);
        render();
        SDL_Delay(50);
    }
}

void App::doStateSettings()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show settings page");
}

void App::doStateAbout()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show about page");
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%d",
                showOptions({"This", "feature", "has", "not", "been", "implemented", "yet"}, 200, 200, 0, FONT_SIZE_XL));
    state = STATE_MENU;
}

// FUNCTION DEFINITIONS END ============================================================================================

#endif //PROJECTNUMA_PROJECTNUMA_H
