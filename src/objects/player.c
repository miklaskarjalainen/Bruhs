#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "player.h"

#include "../globals.h"
#include "../levels.h"

#define PLAYER_GRAVITY   380.0f
#define PLAYER_ACCEL     440.0f
#define PLAYER_DEACCEL   0.15f
#define PLAYER_JUMP_STR -150.0f
#define PLAYER_JUMP_STR_HOLD -205.0f
#define PLAYER_RUN_CAP   140.0f
#define PLAYER_WALK_CAP  80.0f

struct player_t gPlayer = {
	.obj = {
		.position = {32.f, .0f},
		.motion = {.0f, .0f},
		.type = OBJ_PLAYER,
		.is_grounded = false,
	},
	.state = STATE_SMALL,
	.speed_cap = PLAYER_WALK_CAP,
};

void PlayerReset(player_t* player)
{
	player->obj = (object_t){
		.position = {.0f, .0f},
		.motion = (Vector2){ .0f, .0f },
		.type = OBJ_PLAYER,
		.is_grounded = false,
	};
	player->state = STATE_SMALL;
	player->speed_cap = PLAYER_WALK_CAP;
}

void PlayerDraw(player_t* player)
{
	Rectangle rect = {
		player->obj.position.x,
		player->obj.position.y,
		16.f, 16.f
	};

	DrawRectangleRec(rect, RED);
	if (CheckLevelCollision(rect, gCurrentLevel))
	{
		DrawRectangleRec(rect, GREEN);
	}
	DrawRectangleLines(
		player->obj.position.x + 4,
		player->obj.position.y + 1,
		8,
		16,
		DARKBLUE
	);
}

void PlayerUpdate(player_t* player)
{
	const float delta = fminf(GetFrameTime(), 1.0/60.0); // Fix frametime spikes when doing window events (dragging/resizing)
	player->obj.motion.y += PLAYER_GRAVITY * delta;

	if (IsKeyPressed(KEY_R))
		PlayerReset(player);

	// Horizontal input
	float moveX = .0f;
	if (IsKeyDown(KEY_A))
		moveX -= PLAYER_ACCEL * delta;
	if (IsKeyDown(KEY_D))
		moveX += PLAYER_ACCEL * delta;
	player->obj.motion.x += moveX;

	if (player->obj.is_grounded)
	{
		// Slowdown 
		if (moveX == .0f)
		{
			player->obj.motion.x = Lerp(player->obj.motion.x, .0f, PLAYER_DEACCEL);
			if (fabsf(player->obj.motion.x) <= 1.0)
				player->obj.motion.x = .0f;
		}

		// Jump
		if (IsKeyPressed(KEY_K))
			player->obj.motion.y = PLAYER_JUMP_STR;
		
		if (IsKeyDown(KEY_J))
			player->speed_cap = PLAYER_RUN_CAP;
		else
			player->speed_cap = PLAYER_WALK_CAP;
	}
	else
	{
		// Jumpholding
		if (IsKeyDown(KEY_K))
			player->obj.motion.y += PLAYER_JUMP_STR_HOLD * delta;
	}
	
	// Speed caps
	player->obj.motion.x = fmax(fminf(player->obj.motion.x, player->speed_cap), -player->speed_cap);
	
	ObjectMoveAndSlide(&player->obj, (Rectangle) { 0.f, 0.f, 16.f, 16.f});
	float cameraX = fabsf(gCamera.offset.x / gCamera.zoom);
	if (cameraX > player->obj.position.x)
	{
		player->obj.position.x = cameraX;
		player->obj.motion.x = .0f;
	}
	
	// Camera follow
	gCamera.offset.x = -(player->obj.position.x * gCamera.zoom) + ((SCREEN_WIDTH / 2.0) * gCamera.zoom);
}

// TODO: Different sizes.
Rectangle GetPlayerHitbox(const player_t* player)
{
	assert(player->obj.type == OBJ_PLAYER);
	return (Rectangle) { .x = player->obj.position.x, player->obj.position.y, 16.f, 16.f };
}
