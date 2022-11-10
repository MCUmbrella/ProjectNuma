//
// Created by mcumbrella on 22-11-8.
//

#ifndef PROJECTNUMA_PROJECTNUMA_H

#include <SDL_render.h>
#include <SDL_image.h>
#include <cassert>
#include <map>
#include <memory>
#include <list>
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
