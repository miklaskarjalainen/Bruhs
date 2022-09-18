#include <assert.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

#include "player.h"

#include "../assets.h"
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
		.dir = DIR_RIGHT
	},
	.state = STATE_SMALL,
	.speed_cap = PLAYER_WALK_CAP,

	.is_big = false,
	.is_fire = false,
	.is_crouching = false
};

void PlayerReset(player_t* player)
{
	player->obj = (object_t){
		.position = {.0f, .0f},
		.motion = (vec2b){0,0},
		.type = OBJ_PLAYER,
		.is_grounded = false,
		.dir = DIR_RIGHT
	};
	player->state = STATE_SMALL;
	player->speed_cap = PLAYER_WALK_CAP;

	player->is_big = false;
	player->is_fire = false;
	player->is_crouching = false;
}

void PlayerDraw(const player_t* player)
{
	Rectangle rect = {
		player->obj.position.x,
		player->obj.position.y,
		16.f, 16.f
	};
	// DrawRectangleRec(rect, RED);

	const Rectangle Dest = {
		.x = player->obj.position.x,
		.y = player->obj.position.y - 16.f,
		.width = 16.f, .height = 32.f
	};
	const Vector2 Pivot = { .x = 0.f, .y = 0.f };

	if (player->is_big)
	{
		DrawTexturePro(PlayerSprite, PLAYER_BIG_IDLE(player->obj.dir), Dest, Pivot, 0.f, WHITE);
	}
	else
	{
		DrawTexturePro(PlayerSprite, PLAYER_SMALL_IDLE(player->obj.dir), Dest, Pivot, 0.f, WHITE);
	}

}

void PlayerUpdate(player_t* player)
{
	player->speed_timer--;

	if (IsKeyPressed(KEY_R))
		PlayerReset(player);
	player->is_big = IsKeyDown(KEY_SPACE);

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
			player->obj.dir = (dir_t)moveX;
		
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
	
	// ObjectMoveAndSlide(&player->obj, (Rectangle) { 0.f, 0.f, 16.f, 16.f});
	PlayerMoveAndSlide(player);
	float cameraX = fabsf(gCamera.offset.x / gCamera.zoom);
	if (cameraX > player->obj.position.x)
	{
		player->obj.position.x = cameraX;
		player->obj.motion.x = 0;;
	}
	
	// Camera follow
	gCamera.offset.x = -(player->obj.position.x * gCamera.zoom) + ((SCREEN_WIDTH / 2.0) * gCamera.zoom);
}

void PlayerMoveAndSlide(player_t* player)
{
	player->obj.position.x += player->obj.motion.x * SUBPIXEL;
	player->obj.position.y += player->obj.motion.y * SUBPIXEL;

	// Start
	if (player->obj.position.y > (207.f + 16.f)) // currently mario's pivot in a wrong position.
		return;

	// TODO: Other offsets (than just small mario)
	// Get Offsets
	
	const vec2f Pos = (vec2f){ player->obj.position.x, player->obj.position.y - 16};
	const Rectangle Head      = (Rectangle){ Pos.x +  8.f, Pos.y + 18.f, 1.f, 1.f};
	const Rectangle LeftFoot  = (Rectangle){ Pos.x +  3.f, Pos.y + 32.f, 1.f, 1.f};
	const Rectangle RightFoot = (Rectangle){ Pos.x + 12.f, Pos.y + 32.f, 1.f, 1.f };

	const Rectangle hRightSide  = (Rectangle){ Pos.x + 13.f, Pos.y + 8.f, 1.f, 1.f};
	const Rectangle hLeftSide   = (Rectangle){ Pos.x +  2.f, Pos.y + 8.f, 1.f, 1.f};
	const Rectangle lRightSide  = (Rectangle){ Pos.x + 13.f, Pos.y + 24.f, 1.f, 1.f};
	const Rectangle lLeftSide   = (Rectangle){ Pos.x +  2.f, Pos.y + 24.f, 1.f, 1.f};

	// Visual
	DrawRectangleRec(Head, GREEN);
	DrawRectangleRec(LeftFoot, MAGENTA);
	DrawRectangleRec(RightFoot, MAGENTA);
	DrawRectangleRec(lLeftSide, MAGENTA);
	DrawRectangleRec(lRightSide, MAGENTA);

	player->obj.is_grounded = false;

	// Head Check
	if (Pos.y >= 16) // 32 if big
	{
		vec2b tilePos = { (int)(Head.x / 16.f), (int)(Head.y / 16.f) };
		block_type t = GetBlockAt(tilePos.x, tilePos.y);
		if (t != BLOCK_AIR)
		{
			if (t == BLOCK_COIN) // TODO: collect
			{
				player->coin_count++;
				SetLevelBlock(tilePos.x, tilePos.y, BLOCK_AIR);
				return;
			}

			if (player->obj.motion.y < 0.f) // upwards
			{
				const int BelowTile = ((int)Head.y) % 16;
				if (BelowTile >= 4)
				{
					// check for hard tiles (air and coin already checked earlier.)
					player->obj.motion.y = 1;
				}
			}
		}
	}

	// Foot Check
	{
		vec2b tilePos = { (int)(LeftFoot.x / 16.f), (int)(LeftFoot.y / 16.f) };
		block_type atFoot = GetBlockAt(tilePos.x, tilePos.y);
		int footY = (int)LeftFoot.y;
		// Left Foot Check
		{
			if (atFoot == BLOCK_COIN)
			{
				player->coin_count++;
				SetLevelBlock(tilePos.x, tilePos.y, BLOCK_AIR);
				return;
			}

			if (atFoot == BLOCK_AIR)
			{
				// Right Foot Check
				atFoot = GetBlockAt((int)(RightFoot.x / 16.f), (int)(RightFoot.y / 16.f));
				footY = (int)RightFoot.y;
				if (atFoot == BLOCK_COIN) // TODO: Collect 
					return;
				if (atFoot == BLOCK_AIR)
					goto side_check;
			}

		}

		if (player->obj.motion.y > 0) // downwards
		{
			const bool IsInvisible = false; //TODO: invisible block
			if (IsInvisible)
				goto side_check;

			const int BelowTile = footY % 16;
			if (BelowTile <= 4)
			{
				// land on tile
				player->obj.position.y = ((int)(player->obj.position.y / 16.f) * 16);
				player->obj.motion.y = 0;
				player->obj.is_grounded = true;
			}
			else
			{
				// TODO: push out of block
			}
		}
	}


side_check:;
	block_type sideBlock = BLOCK_AIR;
	dir_t colDir = DIR_NONE;
	vec2b tilePos = { 0,0 };
	{
		// High
		if (player->is_big)
		{
			tilePos = (vec2b){(int)(hLeftSide.x / 16.f), (int)(hLeftSide.y / 16.f)};
			sideBlock = GetBlockAt(tilePos.x, tilePos.y);
			if (sideBlock != BLOCK_AIR)
			{
				colDir = DIR_LEFT;
				goto finish_side_check;
			}
			tilePos = (vec2b){ (int)(hRightSide.x / 16.f), (int)(hRightSide.y / 16.f) };
			sideBlock = GetBlockAt(tilePos.x, tilePos.y);
			if (sideBlock != BLOCK_AIR)
			{
				colDir = DIR_RIGHT;
				goto finish_side_check;
			}
		}

		// Low
		tilePos = (vec2b){ (int)(lLeftSide.x / 16.f), (int)(lLeftSide.y / 16.f) };
		sideBlock = GetBlockAt(tilePos.x, tilePos.y);
		if (sideBlock != BLOCK_AIR)
		{
			colDir = DIR_LEFT;
			goto finish_side_check;
		}

		tilePos = (vec2b){ (int)(lRightSide.x / 16.f), (int)(lRightSide.y / 16.f) };
		sideBlock = GetBlockAt(tilePos.x, tilePos.y);
		if (sideBlock != BLOCK_AIR)
		{
			colDir = DIR_RIGHT;
			goto finish_side_check;
		}
	}

finish_side_check:
	if (colDir == DIR_NONE)
		return;
	if (sideBlock == BLOCK_COIN)
	{
		player->coin_count++;
		SetLevelBlock(tilePos.x, tilePos.y, BLOCK_AIR);
		return;
	}
	
	// push out of block
	if (player->obj.motion.x == 0 || colDir == abs(player->obj.motion.x) / player->obj.motion.x)
	{
		player->obj.motion.x = 0;
		player->obj.position.x += (colDir * -1);
	}
}

// TODO: Different sizes.
Rectangle GetPlayerHitbox(const player_t* player)
{
	assert(player->obj.type == OBJ_PLAYER);
	return (Rectangle) { .x = player->obj.position.x, player->obj.position.y, 16.f, 16.f };
}
