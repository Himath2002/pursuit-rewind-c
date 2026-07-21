#ifndef PURSUIT_REWIND_MAP_LOADER_H
#define PURSUIT_REWIND_MAP_LOADER_H

#include "pursuit_rewind/game.h"

/** Result of validating and decoding one map file. */
typedef enum {
    MAP_LOAD_OK,
    MAP_LOAD_OPEN_FAILED,
    MAP_LOAD_INVALID_DIMENSIONS,
    MAP_LOAD_TRUNCATED,
    MAP_LOAD_INVALID_CELL,
    MAP_LOAD_DUPLICATE_ACTOR,
    MAP_LOAD_MISSING_ACTOR,
    MAP_LOAD_TRAILING_DATA
} MapLoadResult;

MapLoadResult map_loader_read(const char *path, GameState *state);
const char *map_loader_result_message(MapLoadResult result);

#endif

