#ifndef OBJECT_H
#define OBJECT_H

#include <raylib.h>

//! If the order of these is changed, change GetObjectHitbox(...) aswell!! Keep player last!
typedef enum object_type
{
	OBJ_NULL   = 0,
	OBJ_GOOMBA = 1,
	OBJ_POWERUP,
	OBJ_PLAYER,
} object_type;

typedef enum dir_t
{
	DIR_LEFT = -1,
	DIR_NONE = 0,
	DIR_RIGHT = 1,
} dir_t;

typedef struct object_t 
{
	object_type type;
	Vector2 position;
	Vector2 motion;
	dir_t dir;
	bool is_grounded; /* Get's updated after move and slide */
} object_t;

bool ObjectSpawn(const object_t obj);
void ObjectsUpdateAndDraw();
Vector2 ObjectMoveAndSlide(object_t* obj, const Rectangle h);
Rectangle GetObjectHitbox(const object_t* obj);

#endif
