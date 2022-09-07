#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include "globals.h"
#include "player.h"
#include "levels.h"

#define PLAYER_GRAVITY   380.0f
#define PLAYER_ACCEL     440.0f
#define PLAYER_DEACCEL   0.01f
#define PLAYER_JUMP_STR -150.0f
#define PLAYER_JUMP_STR_HOLD -180.0f
#define PLAYER_RUN_CAP   140.0f
#define PLAYER_WALK_CAP  80.0f

struct player_t gPlayer = {
	.obj = {
		.position = {32.f, .0f},
		.type = OBJ_PLAYER
	},
	.state = STATE_SMALL,
	.motion = {.0f, .0f}
};

void PlayerReset(player_t* player)
{
	player->obj = (object_t){
		.position = {.0f, .0f},
		.type = OBJ_PLAYER,
		
	};
	player->state = STATE_SMALL;
	player->motion = (Vector2){ .0f, .0f };
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

static bool PlayerIsGrounded(const player_t* player)
{
	Rectangle rect = {
		.x = player->obj.position.x + 4,
		.y = player->obj.position.y + 1,
		.width = 8,
		.height = 16
	};
	return CheckLevelCollision(rect, gCurrentLevel);
}

void PlayerUpdate(player_t* player)
{
	const float delta = fminf(GetFrameTime(), 1.0/60.0); // Fix frametime spikes when doing window events (dragging/resizing)
	player->motion.y += PLAYER_GRAVITY * delta;

	if (IsKeyPressed(KEY_R))
		PlayerReset(player);

	float moveX = .0f;
	if (IsKeyDown(KEY_A))
		moveX -= PLAYER_ACCEL * delta;
	if (IsKeyDown(KEY_D))
		moveX += PLAYER_ACCEL * delta;
	if (PlayerIsGrounded(player) && moveX == .0f)
	{
		player->motion.x = Lerp(player->motion.x, .0f, PLAYER_DEACCEL);
		if (fabsf(player->motion.x) <= 1.0)
			player->motion.x = .0f;
	}

	player->motion.x += moveX;
	if (IsKeyPressed(KEY_W) && PlayerIsGrounded(player))
		player->motion.y = PLAYER_JUMP_STR;
	
	// Speed caps
	player->motion.x = fmax(fminf(player->motion.x, PLAYER_WALK_CAP), -PLAYER_WALK_CAP);
	
	player->motion = ObjectMoveAndSlide(&player->obj, player->motion, (Rectangle) { 0.f, 0.f, 16.f, 16.f });
	float cameraX = fabsf(gCamera.offset.x / gCamera.zoom);
	if (cameraX > player->obj.position.x)
	{
		player->obj.position.x = cameraX;
		player->motion.x = .0f;
	}
	
	if (IsKeyDown(KEY_RIGHT))
		gCamera.offset.x -= (166.0 * gCamera.zoom) * delta;
	if (IsKeyDown(KEY_LEFT))
		gCamera.offset.x += (166.0 * gCamera.zoom) * delta;
}
