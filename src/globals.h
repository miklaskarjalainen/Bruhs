#ifndef GLOBALS_H
#define GLOBALS_H

#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 240
#define OBJ_COUNT 4

extern struct Camera2D gCamera;
extern const struct level_t* gCurrentLevel;
extern struct object_t gObjects[OBJ_COUNT];

#endif
