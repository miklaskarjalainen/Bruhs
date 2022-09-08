#include <assert.h>
#include <raylib.h>
#include <math.h>
#include "../globals.h"
#include "../levels.h"
#include "object.h"

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

void UpdateGoomba(object_t* obj)
{
	assert(obj->type == OBJ_GOOMBA);

	const float delta = fminf(GetFrameTime(), 1.0 / 60.0); // Fix frametime spikes when doing window events (dragging/resizing)

	obj->motion.x = 1400.0f * delta * obj->dir;
	obj->motion.y += 330.0f * delta;
	
	ObjectMoveAndSlide(obj, (Rectangle) { 0.f, 0.f, 16.f, 16.f });
	if (obj->motion.x == .0f)
		obj->dir *= -1.f;
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
			UpdateGoomba(obj);
			DrawRectangleV(obj->position, (Vector2) { 16.f, 16.f }, ORANGE);
			continue;
		}
		default:
			continue;
		}
	}
}

inline bool ObjectIsGrounded(const object_t* obj)
{
	/*
		We but a thin hitbox 1 px below the object,
		use it against the level and if collision is found then 
		the object is grounded.
	*/
	Rectangle rect = {
		.x = obj->position.x + 4,
		.y = obj->position.y + 1,
		.width = 8,
		.height = 16
	};
	return CheckLevelCollision(rect, gCurrentLevel);
}

// TODO: hitboxes with offset
Vector2 ObjectMoveAndSlide(object_t* obj, const Rectangle h)
{
	const float delta = GetFrameTime();

	// X motion
	obj->position.x += obj->motion.x * delta;
	
	Rectangle rect = {
		.x = h.x + obj->position.x,
		.y = h.y + obj->position.y,
		.width  = h.width,
		.height = h.height,
	};
	if (CheckLevelCollision(rect, gCurrentLevel))
	{
		const int xGrid = (int)(rect.x / BLOCK_SIZE);
		if (obj->motion.x > 0.0f)
		{
			obj->position.x = ((xGrid + 1) * BLOCK_SIZE) - rect.width - h.x;
		}
		else
		{
			obj->position.x = (xGrid * BLOCK_SIZE) + BLOCK_SIZE - h.x;
		}
		obj->motion.x = .0f;
	}

	obj->position.y += obj->motion.y * delta;
	rect.x = h.x + obj->position.x;
	rect.y = h.y + obj->position.y;

	if (CheckLevelCollision(rect, gCurrentLevel))
	{
		const int yGrid = (int)(rect.y / BLOCK_SIZE);
		if (obj->motion.y > 0.0f)
		{
			obj->position.y = ((yGrid + 1) * BLOCK_SIZE) - rect.height - h.y;
		}
		else
		{
			obj->position.y = (yGrid * BLOCK_SIZE) + BLOCK_SIZE - h.y;
		}
		obj->motion.y = .0f;
	}

	obj->is_grounded = ObjectIsGrounded(obj);

	return obj->motion;
}
