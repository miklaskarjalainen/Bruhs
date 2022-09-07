#include <raylib.h>
#include "globals.h"
#include "levels.h"

Camera2D gCamera = {
	.target = {.0f, .0f},
	.offset = {.0f, .0f},
	.rotation = .0f,
	.zoom = 1.0f,
};
const level_t* gCurrentLevel = (level_t*)0;
