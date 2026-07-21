#ifndef PURSUIT_REWIND_MOVEMENT_H
#define PURSUIT_REWIND_MOVEMENT_H

#include <stdbool.h>

#include "pursuit_rewind/game.h"

bool movement_is_direction_key(int input);
void movement_apply_player_direction(GameState *state, int input);
void movement_advance_pursuer(GameState *state, int random_direction);

#endif

