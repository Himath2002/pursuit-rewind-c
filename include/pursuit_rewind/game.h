#ifndef PURSUIT_REWIND_GAME_H
#define PURSUIT_REWIND_GAME_H

#include <stdbool.h>

#define PR_MIN_DIMENSION 5
#define PR_MAX_ROWS 60
#define PR_MAX_COLUMNS 160

/** A coordinate in the playable toroidal grid. */
typedef struct {
    int row;
    int column;
} Position;

/** Current lifecycle state of a loaded pursuit session. */
typedef enum {
    GAME_RUNNING,
    GAME_PLAYER_WON,
    GAME_PLAYER_CAUGHT,
    GAME_QUIT
} GameStatus;

/** Complete state reconstructed by map loading and undo snapshots. */
typedef struct {
    int rows;
    int columns;
    Position player;
    Position pursuer;
    Position target;
    GameStatus status;
} GameState;

/** Returns whether dimensions satisfy the supported map contract. */
bool game_dimensions_are_valid(int rows, int columns);

/** Returns whether two positions identify the same grid cell. */
bool game_positions_equal(Position first, Position second);

/** Returns whether a position belongs to the loaded playable grid. */
bool game_position_is_valid(const GameState *state, Position position);

/** Resolves victory before capture for the current actor positions. */
void game_update_status(GameState *state);

/** Returns the user-facing message associated with a game status. */
const char *game_status_message(GameStatus status);

#endif
