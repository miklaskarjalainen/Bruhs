#include "assets.h"

Texture PlayerSprite = { 0 };
Texture TileSetSprite = { 0 };

void LoadAssets()
{
	PlayerSprite  = LoadTexture("./assets/mario.png");
	TileSetSprite = LoadTexture("./assets/tileset.png");
}
