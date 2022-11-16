# Introduction
This game is still in the very early stages of development.
You're expected to experience various bugs or missing features.
## Progress
- [x] Basic gameplay
- [ ] Artwork
- [ ] UI
- [ ] Game menus
- [ ] Weapon & ship upgrade purchase
- [ ] Level system
- [ ] Custom levels
- [ ] Storyline
## Control
- Move: `W` `A` `S` `D`
- Fast / slow move: `right Shift` / `right Ctrl`
- Fire: `SPACE`
- Switch weapon: `right Alt`
- Quit: `Alt+F4` or close the window
## Player
- Size: `40 * 32`
- Speed: `6`, `4.2 (slow moving)`, `9 (fast moving)`
- HP: `50`
## Enemies
### Enemy 0
- Name: `Turret`
- Size: `32 * 24`
- Speed: `2 ± 1`X, `±0.5`Y
- HP: `5`
- Weapon: `#0E`
### Enemy 1
- Name: `Fighter`
- Size: `40 * 32`
- Speed: `1 ± 0.1`X, `±0.1`Y
- HP: `30`
- Weapon: `#1E`
### Enemy 2
- Name: `Armored fighter`
- Size: `60 * 40`
- Speed: `1 ± 0.1`X, `±0.1`Y
- HP: `60`
- Weapon: `#1E`
### Enemy 3
- Name: `Cruiser`
- Size: `60 * 40`
- Speed: `0.25`X, `0`Y
- HP: `750`
- Weapon: `#2E`
## Weapons
### Weapon 0
- Name: `Small machine gun`
- Reload time: `10`
- Bullet speed: `20`
- Bullet damage: `10`
- Bullet size: `8 * 8`
- Description: `Starter weapon`
### Weapon 1
- Name: `Shotgun`
- Reload time: `20`
- Bullet speed: `20`
- Bullet damage: `5`
- Bullet size: `8 * 8`
- Description: `Fire 5 bullets into the area on the right`
### Weapon 2
- Name: `Blaster`
- Reload time: `100`
- Bullet speed: `20`
- Bullet damage: `20`
- Bullet size: `16 * 16`
- Description: `Fire 36 bullets in all directions`
### Weapon 3
- Name: `Laser cannon`
- Reload time: `60`
- Bullet speed: `50`
- Bullet damage: `100`
- Bullet size: `200 * 20`
- Description: `Fires a very high damage laser beam`
### Weapon 0E
- Name: `Turret gun`
- Reload time: `100`
- Bullet speed: `10`
- Bullet damage: `5`
- Bullet size: `8 * 8`
- Description: `Weapon used by enemy turrets`
### Weapon 1E
TODO
### Weapon 2E
TODO
# Build
## Requirements
- libsdl2-dev
- libsdl2-gfx-dev
- libsdl2-image-dev
- libsdl2-mixer-dev
- libsdl2-net-dev
- libsdl2-ttf-dev
## Command
```shell
cmake CMakeLists.txt && make
```
# Miscellaneous
## Hack the save file (session.dat)
- Player HP: the first number
- Credits(money): the second number. Currently not in use
- Unlocked weapons: the third number. The game will process it as binary bits.
  After converting this number to binary, if the last digit is 1, then unlock the weapon 0.
  Check forward bit by bit like this. For example, 5 -> 0101 -> unlock weapon 0 & 2
- Unlocked levels: the forth number. Currently not in use
