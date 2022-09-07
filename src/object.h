#ifndef OBJECT_H
#define OBJECT_H

#include <raylib.h>

typedef enum object_type 
{
	OBJ_POWERUP,
	OBJ_PLAYER,
	OBJ_GOOMBA,
} object_type;

typedef struct object_t 
{
	object_type type;
	Vector2 position;
	bool is_grounded; /* Get's updated after move and slide */
} object_t;

Vector2 ObjectMoveAndSlide(object_t* obj, Vector2 motion, const Rectangle h);

#endif
