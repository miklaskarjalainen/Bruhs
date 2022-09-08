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
	float speed_cap;
} player_t;

extern player_t gPlayer;

void PlayerReset(player_t* player);
void PlayerDraw(player_t* player);
void PlayerUpdate(player_t* player);

#endif
