//
// Created by mcumbrella on 22-11-8.
//

#ifndef PROJECTNUMA_PROJECTNUMA_H

#include <SDL_render.h>
#include <SDL_image.h>
#include <map>
#include <memory>
#include <list>
#include <stdexcept>
#include <string>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
typedef char EntityType;
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

const char* soundFiles[]={
        "assets/projectnuma/sounds/ambient/click0.wav",
        "assets/projectnuma/sounds/ambient/explode0.wav",
        "assets/projectnuma/sounds/ambient/explode1.wav",
        "assets/projectnuma/sounds/ambient/msg.wav",
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
        "assets/projectnuma/sounds/item/weapon3.wav"
};

const char* musicFiles[]={
        "assets/projectnuma/sounds/music/test.ogg"
};

class App;

class Entity;

class Player;

class Enemy0;

class Enemy1;

class Enemy2;

class Enemy3;

class Bullet;

class Weapon;

#define PROJECTNUMA_PROJECTNUMA_H

#endif //PROJECTNUMA_PROJECTNUMA_H
