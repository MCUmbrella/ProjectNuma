//
// Created by mcumbrella on 23-5-8.
//

#include "Player.h"
#include "../App.h"
#include "../RenderManager.h"
#include "../weapon/Weapon.h"
#include "../SoundManager.h"
#include "../Session.h"

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
    texture = App::getInstance().renderManager->getTexture("assets/projectnuma/textures/entity/player.png");
    name = "Player";
    onSpawn = [](Entity* self) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player spawned");
    };
    beforeTick = [&](Entity* self) {
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
                    App::getInstance().currentLevel->isFailed = true;
                }
            }
            return;
        }
        p->dx = p->dy = 0;
        // press W A S D to move, right-Shift to move faster, right-Ctrl to move slower
        if (App::getInstance().pressedKey[SDL_SCANCODE_RSHIFT])
            p->speedModifier = 1.5;
        else if (App::getInstance().pressedKey[SDL_SCANCODE_RCTRL])
            p->speedModifier = 0.7;
        if (App::getInstance().pressedKey[SDL_SCANCODE_W])
            p->dy = -p->speed * p->speedModifier;
        if (App::getInstance().pressedKey[SDL_SCANCODE_A])
            p->dx = -p->speed * p->speedModifier;
        if (App::getInstance().pressedKey[SDL_SCANCODE_S])
            p->dy = p->speed * p->speedModifier;
        if (App::getInstance().pressedKey[SDL_SCANCODE_D])
            p->dx = p->speed * p->speedModifier;
        // press SPACE to fire
        if (p->weapon != null && App::getInstance().pressedKey[SDL_SCANCODE_SPACE] && p->reloadTicks == 0)
        {
            p->weapon->fire(p, 0);
            p->reloadTicks = p->weapon->reloadTicks;
        }
        // press right-Alt to switch weapon
        if (App::getInstance().pressedKey[SDL_SCANCODE_RALT])
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
                    App::getInstance().soundManager->playSound("assets/projectnuma/sounds/ambient/weaponLoad.wav");
                    break;
                }
            App::getInstance().pressedKey[SDL_SCANCODE_RALT] = false;
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
    onDeath = [&](Entity* self) {
        App::getInstance().soundManager->playSound("assets/projectnuma/sounds/entity/playerDie.wav");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Player died");
        self->hp = -100;
        ((Player*) self)->life--;
    };
    // load unlocked weapons from session file
    if (1 & App::getInstance().getSession().unlockedWeapons)
        currentWeapons.emplace_back(App::getInstance().getWeapon("PlayerWeapon0"));
    if (2 & App::getInstance().getSession().unlockedWeapons)
        currentWeapons.emplace_back(App::getInstance().getWeapon("PlayerWeapon1"));
    if (4 & App::getInstance().getSession().unlockedWeapons)
        currentWeapons.emplace_back(App::getInstance().getWeapon("PlayerWeapon2"));
    if (8 & App::getInstance().getSession().unlockedWeapons)
        currentWeapons.emplace_back(App::getInstance().getWeapon("PlayerWeapon3"));
    if (currentWeapons.empty())
    {
        App::getInstance().getSession().unlockedWeapons = 1;
        weapon = App::getInstance().getWeapon("PlayerWeapon0");
    }
    else weapon = currentWeapons[0];
}
