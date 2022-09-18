#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <raylib.h>

#define SUBPIXEL (1.f/16.f)
#define PIXEL (1 * 16)
#define OBJ_COUNT 4

//! If the order of these is changed, change GetObjectHitbox(...) aswell!! Keep player last!
typedef enum object_type
{
	OBJ_NULL   = 0,
	OBJ_GOOMBA = 1,
	OBJ_POWERUP,
	OBJ_PLAYER,
} object_type;

// Technically there are 16 different subspeed values so, meaby use a union instead?
typedef struct vec2b {
	int8_t x, y;
} vec2b;
typedef Vector2 vec2f;

typedef enum dir_t
{
	DIR_LEFT = -1,
	DIR_NONE = 0,
	DIR_RIGHT = 1,
} dir_t;

typedef struct object_t 
{
	object_type type;
	vec2f position;
	vec2b motion;
	dir_t dir;
	bool is_grounded; /* Get's updated after move and slide */
} object_t;

bool ObjectSpawn(const object_t obj);
void ObjectsUpdateAndDraw();
vec2b ObjectMoveAndSlide(object_t* obj, const Rectangle h);
Rectangle GetObjectHitbox(const object_t* obj);

extern struct object_t gObjects[OBJ_COUNT];

#endif
