#include <assert.h>
#include <raylib.h>
#include <math.h>

#include "object.h"
#include "goomba.h"

#include "../levels.h"
#include "../globals.h"

object_t gObjects[OBJ_COUNT] = { 0 };

// TODO: make powerups always spawn.
bool ObjectSpawn(const object_t obj)
{
	for (int i = 0; i < OBJ_COUNT; i++)
	{
		if (gObjects[i].type == OBJ_NULL)
		{
			gObjects[i] = obj;
			return true;
		}
	}
	return false;
}

void ObjectsUpdateAndDraw()
{
	int ypos = 40;
	for (int i = 0; i < OBJ_COUNT; i++)
	{
		object_t* obj = &gObjects[i];
		
		DrawText(TextFormat("%u", obj->type), 10, ypos, 18, BLUE);
		ypos += 20;

		switch (obj->type)
		{
		case OBJ_GOOMBA:
		{
			GoombaUpdateAndDraw(obj);
			continue;
		}
		default:
			continue;
		}
	}
}

bool ObjectIsGrounded(const object_t* obj)
{
	/*
		We but a thin hitbox 1 px below the object,
		use it against the level and if collision is found then 
		the object is grounded.

		FIXME: using a thin hitboxi for grounded check doesn't
		make the walljump work.
	*/
	Rectangle rect = {
		.x = obj->position.x + 4,
		.y = obj->position.y + 1,
		.width = 8,
		.height = 16
	};
	return CheckLevelCollision(rect, &gCurrentLevel);
}

// TODO: hitboxes with offset
vec2b ObjectMoveAndSlide(object_t* obj, const Rectangle h)
{
	// X motion
	obj->position.x += obj->motion.x * SUBPIXEL;
	
	Rectangle rect = {
		.x = h.x + obj->position.x,
		.y = h.y + obj->position.y,
		.width  = h.width,
		.height = h.height,
	};
	if (CheckLevelCollision(rect, &gCurrentLevel))
	{
		const int xGrid = (int)(rect.x / BLOCK_SIZE);
		if (obj->motion.x > 0)
		{
			obj->position.x = ((xGrid + 1) * BLOCK_SIZE) - rect.width - h.x;
		}
		else
		{
			obj->position.x = (xGrid * BLOCK_SIZE) + BLOCK_SIZE - h.x;
		}
		obj->motion.x = 0;
	}

	obj->position.y += obj->motion.y * SUBPIXEL;
	rect.x = h.x + obj->position.x;
	rect.y = h.y + obj->position.y;

	if (CheckLevelCollision(rect, &gCurrentLevel))
	{
		const int yGrid = (int)(rect.y / BLOCK_SIZE);
		if (obj->motion.y > 0)
		{
			obj->position.y = ((yGrid + 1) * BLOCK_SIZE) - rect.height - h.y;
		}
		else
		{
			obj->position.y = (yGrid * BLOCK_SIZE) + BLOCK_SIZE - h.y;
		}
		obj->motion.y = 0;
	}

	obj->is_grounded = ObjectIsGrounded(obj);

	return obj->motion;
}

// W/ position (hitbox is different than bounding box)
Rectangle GetObjectHitbox(const object_t* obj)
{
	assert(obj->type < OBJ_PLAYER); // don´t use this with player! Use GetPlayerHitbox instead!

	static Rectangle hb[] = {
		{0.f, 0.f, 0.f, 0.f}, // null
		{2.f, 3.f, 12.f, 10.f}, // goomba
		{0.f, 0.f, 16.f, 16.f}, // next obj
	};

	return (Rectangle){
		.x = obj->position.x + hb[obj->type].x,
		.y = obj->position.y + hb[obj->type].y,
		.width  = hb[obj->type].width,
		.height = hb[obj->type].height
	};
}
