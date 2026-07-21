#ifndef PURSUIT_REWIND_RENDERER_H
#define PURSUIT_REWIND_RENDERER_H

#include <stddef.h>

#include "pursuit_rewind/game.h"

/** Draws the current board, map identity, undo depth, and optional notice. */
void renderer_draw(
        const GameState *state,
        size_t undo_depth,
        const char *map_name,
        const char *notice
);

#endif
