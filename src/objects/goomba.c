#include <assert.h>
#include <math.h>

#include "object.h"
#include "goomba.h"

#include "player.h"

inline object_t CreateGoomba(const Vector2 position)
{
    return (object_t) {
        .type = OBJ_GOOMBA,
            .position = position,
            .motion = (Vector2){ 32.0f, .0f }, // non 0 x because otherwise would get flipped at start
            .is_grounded = false,
            .dir = DIR_LEFT
    };
}

inline void GoombaUpdateAndDraw(object_t* obj)
{
	assert(obj->type == OBJ_GOOMBA);

	const float delta = fminf(GetFrameTime(), 1.0 / 60.0); // Fix frametime spikes when doing window events (dragging/resizing)

	obj->motion.x = 1400.0f * delta * obj->dir;
	obj->motion.y += 330.0f * delta;

	ObjectMoveAndSlide(obj, (Rectangle) { 0.f, 0.f, 16.f, 16.f });
	if (obj->motion.x == .0f)
		obj->dir *= -1.f;
	
	DrawRectangleV(obj->position, (Vector2) { 16.f, 16.f }, ORANGE);

	// TODO: Method to get player's hitbox.
	Rectangle ph = GetPlayerHitbox(&gPlayer);
	Rectangle gh = GetObjectHitbox(obj);
	DrawRectangleRec(gh, BLUE);

	// Stomping
	if (CheckCollisionRecs(ph, gh))
	{
		if (gPlayer.obj.motion.y > 2.0f)
		{
			obj->type = OBJ_NULL;
			gPlayer.obj.motion.y = -120.0f;
		}
	}
}
