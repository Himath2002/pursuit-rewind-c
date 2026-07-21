#include "pursuit_rewind/movement.h"

#include <ctype.h>
#include <stdlib.h>

static int wrap_coordinate(int value, int maximum)
{
    if (value < 1) {
        return maximum;
    }
    if (value > maximum) {
        return 1;
    }
    return value;
}

static int step_toward(int difference)
{
    if (difference > 0) {
        return -1;
    }
    if (difference < 0) {
        return 1;
    }
    return 0;
}

bool movement_is_direction_key(int input)
{
    int normalized = tolower((unsigned char)input);
    return normalized == 'w'
            || normalized == 'a'
            || normalized == 's'
            || normalized == 'd';
}

void movement_apply_player_direction(GameState *state, int input)
{
    int normalized;

    if (state == NULL || state->status != GAME_RUNNING) {
        return;
    }

    normalized = tolower((unsigned char)input);
    switch (normalized) {
        case 'w':
            state->player.row = wrap_coordinate(state->player.row - 1, state->rows);
            break;
        case 's':
            state->player.row = wrap_coordinate(state->player.row + 1, state->rows);
            break;
        case 'a':
            state->player.column =
                    wrap_coordinate(state->player.column - 1, state->columns);
            break;
        case 'd':
            state->player.column =
                    wrap_coordinate(state->player.column + 1, state->columns);
            break;
        default:
            break;
    }
}

void movement_advance_pursuer(GameState *state, int random_direction)
{
    static const Position direction_delta[8] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1},
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1}
    };
    Position candidate;
    int row_difference;
    int column_difference;

    if (state == NULL || state->status != GAME_RUNNING) {
        return;
    }

    row_difference = state->pursuer.row - state->player.row;
    column_difference = state->pursuer.column - state->player.column;
    candidate = state->pursuer;

    if (abs(row_difference) <= 1 && abs(column_difference) <= 1) {
        candidate.row += step_toward(row_difference);
        candidate.column += step_toward(column_difference);
    } else if (random_direction >= 0 && random_direction < 8) {
        candidate.row += direction_delta[random_direction].row;
        candidate.column += direction_delta[random_direction].column;
        candidate.row = wrap_coordinate(candidate.row, state->rows);
        candidate.column = wrap_coordinate(candidate.column, state->columns);
    }

    if (!game_positions_equal(candidate, state->target)) {
        state->pursuer = candidate;
    }
}

