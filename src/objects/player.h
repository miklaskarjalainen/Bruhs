/*
	BTW:
		Bounding box of the player is the same size as the hitbox of the player.
		For most other objects (like enemies) this changes.
*/

#ifndef PLAYER_H
#define PLAYER_H
#include "object.h"

typedef enum player_state {
	STATE_SMALL = 0b00,
	STATE_BIG   = 0b01,
	STATE_FIRE  = 0b10,
} player_state;

typedef struct player_t {
	object_t obj;
	player_state state;
	char speed_cap;
	char speed_timer; // when reaches 0 and run not held, resets cap to walkspd.
	
	bool is_big;
	bool is_fire;
	bool is_crouching;
} player_t;

extern player_t gPlayer;

void PlayerReset(player_t* player);
void PlayerDraw(const player_t* player);
void PlayerUpdate(player_t* player);
void PlayerMoveAndSlide(player_t* player);
Rectangle GetPlayerHitbox(const player_t* player);

#endif
