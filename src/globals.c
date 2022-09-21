#include <raylib.h>
#include "globals.h"
#include "levels.h"

Camera2D gCamera = {
	.target = {.0f, .0f},
	.offset = {.0f, .0f},
	.rotation = .0f,
	.zoom = 1.0f,
};

level_t gCurrentLevel = {
	.data = (char*)0,
	.width = 0
};

char gAnimFrame = 0;
