#include "assets.h"

Texture PlayerSprite = { 0 };

void LoadAssets()
{
	PlayerSprite = LoadTexture("./assets/mario.png");
}
