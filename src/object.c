#include "globals.h"
#include "levels.h"
#include "object.h"

// TODO: hitboxes with offset
Vector2 ObjectMoveAndSlide(object_t* obj, Vector2 motion, const Rectangle h)
{
	// X motion
	obj->position.x += motion.x;
	
	Rectangle rect = {
		.x = h.x + obj->position.x,
		.y = h.y + obj->position.y,
		.width  = h.width,
		.height = h.height,
	};
	if (CheckLevelCollision(rect, gCurrentLevel))
	{
		const int xGrid = (int)(rect.x / BLOCK_SIZE);
		if (motion.x > 0.0f)
		{
			obj->position.x = ((xGrid + 1) * BLOCK_SIZE) - rect.width - h.x;
		}
		else
		{
			obj->position.x = (xGrid * BLOCK_SIZE) + BLOCK_SIZE - h.x;
		}
		motion.x = .0f;
	}

	obj->position.y += motion.y;
	rect.x = h.x + obj->position.x;
	rect.y = h.y + obj->position.y;

	if (CheckLevelCollision(rect, gCurrentLevel))
	{
		const int yGrid = (int)(rect.y / BLOCK_SIZE);
		if (motion.y > 0.0f)
		{
			obj->position.y = ((yGrid + 1) * BLOCK_SIZE) - rect.height - h.y;
		}
		else
		{
			obj->position.y = (yGrid * BLOCK_SIZE) + BLOCK_SIZE - h.y;
		}
		motion.y = .0f;
	}

	return motion;
}
