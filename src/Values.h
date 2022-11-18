//
// Created by mcumbrella on 22-11-18.
//

#ifndef PROJECTNUMA_VALUES_H
#define PROJECTNUMA_VALUES_H

#define VERSION "ProjectNuma version alpha 0.0.1"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
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

const char* soundFiles[] = {
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
};

#endif //PROJECTNUMA_VALUES_H
