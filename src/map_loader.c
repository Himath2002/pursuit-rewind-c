#include "pursuit_rewind/map_loader.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TOKEN_READ_OK,
    TOKEN_READ_END,
    TOKEN_READ_INVALID
} TokenReadResult;

static TokenReadResult read_integer_token(FILE *file, int *value)
{
    char token[64];
    char *end;
    long parsed;
    size_t length = 0U;
    int character;
    bool token_is_too_long = false;

    do {
        character = fgetc(file);
    } while (character != EOF && isspace((unsigned char)character));

    if (character == EOF) {
        return TOKEN_READ_END;
    }

    do {
        if (length + 1U < sizeof(token)) {
            token[length++] = (char)character;
        } else {
            token_is_too_long = true;
        }
        character = fgetc(file);
    } while (character != EOF && !isspace((unsigned char)character));

    if (token_is_too_long) {
        return TOKEN_READ_INVALID;
    }

    token[length] = '\0';
    errno = 0;
    end = NULL;
    parsed = strtol(token, &end, 10);
    if (errno == ERANGE
            || end == token
            || *end != '\0'
            || parsed < INT_MIN
            || parsed > INT_MAX) {
        return TOKEN_READ_INVALID;
    }

    *value = (int)parsed;
    return TOKEN_READ_OK;
}

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

    if (read_integer_token(file, &candidate.rows) != TOKEN_READ_OK
            || read_integer_token(file, &candidate.columns) != TOKEN_READ_OK) {
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

            TokenReadResult token_result = read_integer_token(file, &cell);

            if (token_result == TOKEN_READ_END) {
                result = MAP_LOAD_TRUNCATED;
                goto finish;
            }
            if (token_result == TOKEN_READ_INVALID) {
                result = MAP_LOAD_INVALID_CELL;
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
        if (read_integer_token(file, &trailing_value) != TOKEN_READ_END) {
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
