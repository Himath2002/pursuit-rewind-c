#include "pursuit_rewind/renderer.h"

#include <stdio.h>

static char cell_symbol(const GameState *state, Position cell)
{
    if (cell.row == 0
            || cell.row == state->rows + 1
            || cell.column == 0
            || cell.column == state->columns + 1) {
        return '#';
    }
    if (game_positions_equal(cell, state->player)
            && game_positions_equal(cell, state->pursuer)) {
        return 'X';
    }
    if (game_positions_equal(cell, state->player)) {
        return 'P';
    }
    if (game_positions_equal(cell, state->pursuer)) {
        return '~';
    }
    if (game_positions_equal(cell, state->target)) {
        return '@';
    }
    return ' ';
}

void renderer_draw(
        const GameState *state,
        size_t undo_depth,
        const char *map_name,
        const char *notice
)
{
    int row;
    int column;

    if (state == NULL) {
        return;
    }

    printf("\033[2J\033[H");
    printf("PURSUIT REWIND  |  W/A/S/D move  |  U undo  |  Q quit\n");
    printf("Map: %s  |  Undo depth: %zu\n\n",
            map_name == NULL ? "(unknown)" : map_name,
            undo_depth);

    for (row = 0; row <= state->rows + 1; row++) {
        for (column = 0; column <= state->columns + 1; column++) {
            Position cell = {row, column};
            putchar(cell_symbol(state, cell));
        }
        putchar('\n');
    }

    printf("\nP you   ~ pursuer   @ target   # wrap boundary");
    if (notice != NULL && notice[0] != '\0') {
        printf("\n%s", notice);
    }
    if (state->status != GAME_RUNNING) {
        printf("\n\n%s", game_status_message(state->status));
    }
    printf("\n");
    fflush(stdout);
}

