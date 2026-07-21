#include "pursuit_rewind/map_loader.h"

#include <stdio.h>

MapLoadResult map_loader_read(const char *path, GameState *state)
{
    FILE *file;
    GameState candidate = {0};
    MapLoadResult result = MAP_LOAD_OK;
    int player_count = 0;
    int target_count = 0;
    int pursuer_count = 0;
    int row;
    int column;

    if (path == NULL || state == NULL) {
        return MAP_LOAD_OPEN_FAILED;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        return MAP_LOAD_OPEN_FAILED;
    }

    if (fscanf(file, "%d %d", &candidate.rows, &candidate.columns) != 2) {
        result = MAP_LOAD_INVALID_DIMENSIONS;
        goto finish;
    }
    if (!game_dimensions_are_valid(candidate.rows, candidate.columns)) {
        result = MAP_LOAD_INVALID_DIMENSIONS;
        goto finish;
    }

    candidate.status = GAME_RUNNING;
    for (row = 1; row <= candidate.rows; row++) {
        for (column = 1; column <= candidate.columns; column++) {
            int cell;
            Position position = {row, column};

            if (fscanf(file, "%d", &cell) != 1) {
                result = MAP_LOAD_TRUNCATED;
                goto finish;
            }

            switch (cell) {
                case 0:
                    break;
                case 1:
                    player_count++;
                    candidate.player = position;
                    break;
                case 2:
                    target_count++;
                    candidate.target = position;
                    break;
                case 3:
                    pursuer_count++;
                    candidate.pursuer = position;
                    break;
                default:
                    result = MAP_LOAD_INVALID_CELL;
                    goto finish;
            }

            if (player_count > 1 || target_count > 1 || pursuer_count > 1) {
                result = MAP_LOAD_DUPLICATE_ACTOR;
                goto finish;
            }
        }
    }

    {
        int trailing_value;
        int trailing_result = fscanf(file, " %d", &trailing_value);
        if (trailing_result != EOF) {
            result = MAP_LOAD_TRAILING_DATA;
            goto finish;
        }
    }

    if (player_count != 1 || target_count != 1 || pursuer_count != 1) {
        result = MAP_LOAD_MISSING_ACTOR;
        goto finish;
    }

    *state = candidate;

finish:
    fclose(file);
    return result;
}

const char *map_loader_result_message(MapLoadResult result)
{
    switch (result) {
        case MAP_LOAD_OK:
            return "Map loaded.";
        case MAP_LOAD_OPEN_FAILED:
            return "The map file could not be opened.";
        case MAP_LOAD_INVALID_DIMENSIONS:
            return "Map dimensions are missing or outside the supported range.";
        case MAP_LOAD_TRUNCATED:
            return "The map ends before every declared cell is defined.";
        case MAP_LOAD_INVALID_CELL:
            return "A map cell is not one of 0, 1, 2, or 3.";
        case MAP_LOAD_DUPLICATE_ACTOR:
            return "The map defines an actor more than once.";
        case MAP_LOAD_MISSING_ACTOR:
            return "The map must contain one player, one target, and one pursuer.";
        case MAP_LOAD_TRAILING_DATA:
            return "The map contains data beyond its declared dimensions.";
        default:
            return "The map is invalid.";
    }
}

