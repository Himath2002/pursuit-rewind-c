#ifndef PURSUIT_REWIND_MOVEMENT_H
#define PURSUIT_REWIND_MOVEMENT_H

#include <stdbool.h>

#include "pursuit_rewind/game.h"

/** Returns whether input is one of the four case-insensitive directions. */
bool movement_is_direction_key(int input);

/** Applies one player direction with toroidal edge wrapping. */
void movement_apply_player_direction(GameState *state, int input);

/**
 * Advances the pursuer through the shortest adjacent wrapped step, or through
 * the supplied wrapped random direction when the player is farther away.
 */
void movement_advance_pursuer(GameState *state, int random_direction);

#endif
