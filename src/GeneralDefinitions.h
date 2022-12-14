//
// Created by mcumbrella on 22-11-18.
//

#ifndef PROJECTNUMA_GENERALDEFINITIONS_H
#define PROJECTNUMA_GENERALDEFINITIONS_H

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
#define SIDE_PLAYER 0
#define SIDE_ENEMY 1
#define SIDE_ENVIRONMENT 2
#define SIDE_NONE 2147483647

using namespace std;
typedef unsigned char EntityType;
bool flDebug = false, flFullscreen = false, flSoftwareRender = false;

const char* soundFiles[] = {
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

const char* musicFiles[] = {
        "assets/projectnuma/sounds/music/test.ogg",
        "assets/projectnuma/sounds/music/menu/0.ogg",
        "assets/projectnuma/sounds/music/game/0.ogg"
};

const char* textureFiles[] = {
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
    FONT_SIZE_XS, // 16pt
    FONT_SIZE_S, // 24pt
    FONT_SIZE_M, // 32pt
    FONT_SIZE_L, // 40pt
    FONT_SIZE_XL, // 48pt
};

class Entity;

class Player;

class Bullet;

class Weapon;

class App;

#endif //PROJECTNUMA_GENERALDEFINITIONS_H
