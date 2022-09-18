#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

extern Texture PlayerSprite;
extern Texture TileSetSprite;

#define PLAYER_SMALL_IDLE(flip) (Rectangle) {0,0,16.f * flip, 32.f}
#define PLAYER_BIG_IDLE(flip) (Rectangle) {0,64,16.f * flip, 32.f}
#define TILE_GROUND (Rectangle) {0,0,16.f,16.f}

void LoadAssets();

#endif
