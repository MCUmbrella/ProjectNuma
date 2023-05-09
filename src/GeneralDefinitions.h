//
// Created by mcumbrella on 22-11-18.
//

#ifndef PROJECTNUMA_GENERALDEFINITIONS_H
#define PROJECTNUMA_GENERALDEFINITIONS_H

#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <list>
#include <random>
#include <stdexcept>
#include <string>

#define null nullptr
#define VERSION "ProjectNuma version alpha 0.0.1"
#define FONT_FILE "assets/projectnuma/font/MesloLGS_NF_Regular.ttf"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define RENDERER_FLAGS 0
#define WINDOW_FLAGS 0
#define ENTITY_TYPE_PLAYER '\x00'
#define ENTITY_TYPE_ENEMY '\x01'
#define ENTITY_TYPE_BULLET '\x02'
#define ENTITY_TYPE_ENVIRONMENT '\x03'
#define ENTITY_TYPE_ALLY '\x04'
#define ENTITY_TYPE_OTHER '\xff'
#define SIDE_NONE 2147483647

using namespace std;
typedef unsigned char EntityType;

const static char* soundFiles[] = {
        "assets/projectnuma/sounds/ambient/explode0.wav",
        "assets/projectnuma/sounds/ambient/explode1.wav",
        "assets/projectnuma/sounds/ambient/warn.wav",
        "assets/projectnuma/sounds/ambient/weaponLoad.wav",
        "assets/projectnuma/sounds/entity/bossDie.wav",
        "assets/projectnuma/sounds/entity/enemyDie.wav",
        "assets/projectnuma/sounds/entity/playerDie.wav",
        "assets/projectnuma/sounds/entity/hit.wav",
        "assets/projectnuma/sounds/item/missle.wav",
        "assets/projectnuma/sounds/item/weapon0.wav",
        "assets/projectnuma/sounds/item/weapon0e.wav",
        "assets/projectnuma/sounds/item/weapon1.wav",
        "assets/projectnuma/sounds/item/weapon1e.wav",
        "assets/projectnuma/sounds/item/weapon2.wav",
        "assets/projectnuma/sounds/item/weapon2e.wav",
        "assets/projectnuma/sounds/item/weapon3.wav",
        "assets/projectnuma/sounds/misc/click0.wav",
        "assets/projectnuma/sounds/misc/msg.wav",
};

const static char* musicFiles[] = {
        "assets/projectnuma/sounds/music/test.ogg",
        "assets/projectnuma/sounds/music/menu/0.ogg",
        "assets/projectnuma/sounds/music/game/0.ogg"
};

const static char* textureFiles[] = {
        "assets/projectnuma/textures/misc/placeholder.png",
        "assets/projectnuma/textures/misc/black.png",
        "assets/projectnuma/textures/entity/player.png",
        "assets/projectnuma/textures/entity/enemy0.png",
        "assets/projectnuma/textures/entity/enemy1.png",
        "assets/projectnuma/textures/entity/enemy2.png",
        "assets/projectnuma/textures/entity/enemy3.png",
        "assets/projectnuma/textures/entity/weapon0bullet.png",
        "assets/projectnuma/textures/entity/weapon1bullet.png",
        "assets/projectnuma/textures/entity/weapon2bullet.png",
        "assets/projectnuma/textures/entity/weapon3bullet.png",
        "assets/projectnuma/textures/entity/weapon0ebullet.png",
        "assets/projectnuma/textures/entity/weapon1ebullet.png",
        "assets/projectnuma/textures/entity/weapon2ebullet.png",
        "assets/projectnuma/textures/gui/title.png",
};

enum GameState
{
    STATE_STARTUP,
    STATE_MENU,
    STATE_LEVELS,
    STATE_GAME,
    STATE_HANGAR,
    STATE_SETTINGS,
    STATE_ABOUT,
    STATE_SHUTDOWN,
};

enum FontSize
{
    FONT_SIZE_XS = 16, // 16pt
    FONT_SIZE_S = 24, // 24pt
    FONT_SIZE_M = 32, // 32pt
    FONT_SIZE_L = 40, // 40pt
    FONT_SIZE_XL = 48, // 48pt
};

enum EntitySide
{
    SIDE_PLAYER = 0,
    SIDE_ENEMY = 1,
    SIDE_ENVIRONMENT = 2
};

class SoundManager;

class RenderManager;

class Entity;

class Player;

class Bullet;

class Weapon;

class App;

class Session;

class Random;

class Enemy0;

class Enemy1;

class Enemy2;

class Enemy3;

class PlayerWeapon0;

class PlayerWeapon1;

class PlayerWeapon2;

class PlayerWeapon3;

class EnemyWeapon0;

class EnemyWeapon1;

class EnemyWeapon2;

#endif //PROJECTNUMA_GENERALDEFINITIONS_H
