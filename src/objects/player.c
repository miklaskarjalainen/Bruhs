#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "player.h"

#include "../globals.h"
#include "../levels.h"

// https://web.archive.org/web/20130807122227/http://i276.photobucket.com/albums/kk21/jdaster64/smb_playerphysics.png
#define PLAYER_ACCEL 1
#define PLAYER_DEACCEL 1
#define PLAYER_DEACCEL_SKIDDING 2

#define PLAYER_BASE_GRAVITY 7
#define PLAYER_JUMP_STR   (-(PIXEL * 4))
#define PLAYER_JUMP_STR_R (-(PIXEL * 5))
#define PLAYER_FALL_CAP (4 * PIXEL)

#define PLAYER_RUN_CAP   (PIXEL * 2 + 9)
#define PLAYER_WALK_CAP  (PIXEL + 9)

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
		.motion = (vec2b){0,0},
		.type = OBJ_PLAYER,
		.is_grounded = false,
	};
	player->state = STATE_SMALL;
	player->speed_cap = PLAYER_WALK_CAP;
}

void PlayerMoveAndSlide(const player_t* player);
void PlayerDraw(const player_t* player)
{
	Rectangle rect = {
		player->obj.position.x,
		player->obj.position.y,
		16.f, 16.f
	};

	DrawRectangleRec(rect, RED);
	if (CheckLevelCollision(rect, &gCurrentLevel))
	{
		DrawRectangleRec(rect, GREEN);
	}

	PlayerMoveAndSlide(player);
}

void PlayerUpdate(player_t* player)
{
	player->speed_timer--;

	if (IsKeyPressed(KEY_R))
		PlayerReset(player);

	// Horizontal input
	char moveX = 0;
	if (IsKeyDown(KEY_A)) 
		moveX -= 1;
	if (IsKeyDown(KEY_D))
		moveX += 1;
	
	if (moveX != 0)
	{
		char movDir = player->obj.motion.x != 0 ? abs(player->obj.motion.x) / player->obj.motion.x : 0;
		if (player->obj.is_grounded)
		{
			player->obj.dir = (dir_t)moveX;
		}
		if (movDir != player->obj.dir)
			player->obj.motion.x += moveX * PLAYER_DEACCEL_SKIDDING;
		else
			player->obj.motion.x += moveX * PLAYER_ACCEL;
	}
	

	// Gravity
	int gravity = 0;
	int8_t absX = abs(player->obj.motion.x);
	if (IsKeyDown(KEY_K) && player->obj.motion.y < 0)
	{
		if (absX >= (PIXEL * 2))
			gravity = 3;
		else if (absX >= PIXEL)
			gravity = 2;
		else
			gravity = 2;
	}
	else
	{
		if (absX >= (PIXEL * 2))
			gravity = 9;
		else if (absX >= PIXEL)
			gravity = 6;
		else
			gravity = 7;
	}
	
	// Cap falling speed
	if ((player->obj.motion.y + gravity) <= PLAYER_FALL_CAP)
		player->obj.motion.y += gravity;
	else
		player->obj.motion.y = PLAYER_FALL_CAP;

	if (player->obj.is_grounded)
	{
		// Slowdown 
		if (moveX == 0)
		{
			if (player->obj.motion.x > 0)
				player->obj.motion.x -= PLAYER_DEACCEL;
			else if (player->obj.motion.x < 0)
				player->obj.motion.x += PLAYER_DEACCEL;
		}

		// Jump
		if (IsKeyPressed(KEY_K))
		{
			player->obj.motion.y = PLAYER_JUMP_STR;
			if (absX >= ((PIXEL * 2)+5))
				player->obj.motion.y = PLAYER_JUMP_STR_R;
		}
		
		if (IsKeyDown(KEY_J))
		{
			player->speed_cap = PLAYER_RUN_CAP;
			player->speed_timer = 10;
		}
		else if (player->speed_timer == 0)
			player->speed_cap = PLAYER_WALK_CAP;
	}
	else
	{
		if (absX <= PLAYER_WALK_CAP)
			player->speed_cap = PLAYER_WALK_CAP;
	}
	
	
	// Speed caps
	if (player->obj.motion.x > player->speed_cap)
		player->obj.motion.x = player->speed_cap;
	if (player->obj.motion.x < -player->speed_cap)
		player->obj.motion.x = -player->speed_cap;
	
	ObjectMoveAndSlide(&player->obj, (Rectangle) { 0.f, 0.f, 16.f, 16.f});
	float cameraX = fabsf(gCamera.offset.x / gCamera.zoom);
	if (cameraX > player->obj.position.x)
	{
		player->obj.position.x = cameraX;
		player->obj.motion.x = 0;;
	}
	
	// Camera follow
	gCamera.offset.x = -(player->obj.position.x * gCamera.zoom) + ((SCREEN_WIDTH / 2.0) * gCamera.zoom);
}

void PlayerMoveAndSlide(const player_t* player)
{
	// Start
	if (player->obj.position.y > 207.f)
		return;

	// TODO: Other offsets (than just small mario)
	// Get Offsets
	const vec2f Pos = (vec2f){ player->obj.position.x, player->obj.position.y - 16};
	const Rectangle Head      = (Rectangle){ Pos.x +  8.f, Pos.y + 18.f, 1.f, 1.f};
	const Rectangle LeftFoot  = (Rectangle){ Pos.x +  3.f, Pos.y + 32.f, 1.f, 1.f};
	const Rectangle RightFoot = (Rectangle){ Pos.x + 12.f, Pos.y + 32.f, 1.f, 1.f };

	const Rectangle lLeftSide  = (Rectangle){ Pos.x +  2.f, Pos.y + 24.f, 1.f, 1.f};
	const Rectangle lRightSide = (Rectangle){ Pos.x + 13.f, Pos.y + 24.f, 1.f, 1.f};

	// Visual
	DrawRectangleRec(Head, GREEN);
	DrawRectangleRec(LeftFoot, MAGENTA);
	DrawRectangleRec(RightFoot, MAGENTA);
	DrawRectangleRec(lLeftSide, MAGENTA);
	DrawRectangleRec(lRightSide, MAGENTA);

	// Head Check
	if (Pos.y >= 16) // 32 if big
	{
		block_type t = GetBlockAt((int)(Head.x / 16.f), (int)(Head.y / 16.f));
		if (t != BLOCK_AIR)
		{
			if (t == BLOCK_COIN) // TODO: collect
				return;

			if (player->obj.motion.y <= 0.f)
			{
				const int BelowTile = ((int)Head.y) % 16;
				if (BelowTile <= 4)
				{

				}

			}
		}
	}

}

// TODO: Different sizes.
Rectangle GetPlayerHitbox(const player_t* player)
{
	assert(player->obj.type == OBJ_PLAYER);
	return (Rectangle) { .x = player->obj.position.x, player->obj.position.y, 16.f, 16.f };
}
