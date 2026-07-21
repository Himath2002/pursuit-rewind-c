#include "pursuit_rewind/game.h"

#include <stddef.h>

bool game_dimensions_are_valid(int rows, int columns)
{
    return rows >= PR_MIN_DIMENSION
            && rows <= PR_MAX_ROWS
            && columns >= PR_MIN_DIMENSION
            && columns <= PR_MAX_COLUMNS;
}

bool game_positions_equal(Position first, Position second)
{
    return first.row == second.row && first.column == second.column;
}

bool game_position_is_valid(const GameState *state, Position position)
{
    return state != NULL
            && position.row >= 1
            && position.row <= state->rows
            && position.column >= 1
            && position.column <= state->columns;
}

void game_update_status(GameState *state)
{
    if (state == NULL || state->status != GAME_RUNNING) {
        return;
    }

    if (game_positions_equal(state->player, state->target)) {
        state->status = GAME_PLAYER_WON;
    } else if (game_positions_equal(state->player, state->pursuer)) {
        state->status = GAME_PLAYER_CAUGHT;
    }
}

const char *game_status_message(GameStatus status)
{
    switch (status) {
        case GAME_PLAYER_WON:
            return "Target reached. The pursuit timeline is complete.";
        case GAME_PLAYER_CAUGHT:
            return "The pursuer reached your cell. Timeline closed.";
        case GAME_QUIT:
            return "Session ended by player.";
        case GAME_RUNNING:
        default:
            return "Timeline active.";
    }
}

