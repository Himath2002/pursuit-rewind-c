#include "pursuit_rewind/game.h"
#include "pursuit_rewind/history.h"
#include "pursuit_rewind/map_loader.h"
#include "pursuit_rewind/movement.h"
#include "pursuit_rewind/random_source.h"
#include "pursuit_rewind/renderer.h"
#include "pursuit_rewind/terminal.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *program_name)
{
    printf("Usage: %s <map-file>\n", program_name);
    printf("       %s --help\n\n", program_name);
    puts("Map cells: 0 empty, 1 player, 2 target, 3 pursuer");
}

int main(int argc, char *argv[])
{
    GameState state;
    TurnHistory history;
    MapLoadResult map_result;
    const char *notice = "";
    int exit_code = EXIT_SUCCESS;

    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }
    if (argc != 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    map_result = map_loader_read(argv[1], &state);
    if (map_result != MAP_LOAD_OK) {
        fprintf(stderr, "Map error: %s\n", map_loader_result_message(map_result));
        return EXIT_FAILURE;
    }

    history_initialize(&history);
    random_source_seed();

    while (state.status == GAME_RUNNING) {
        char key;
        int normalized;

        renderer_draw(&state, history_size(&history), argv[1], notice);
        notice = "";
        if (!terminal_read_key(&key)) {
            state.status = GAME_QUIT;
            break;
        }

        normalized = tolower((unsigned char)key);
        if (normalized == 'q') {
            state.status = GAME_QUIT;
        } else if (normalized == 'u') {
            notice = history_restore_previous(&history, &state)
                    ? "Previous turn restored."
                    : "No earlier turn is available.";
        } else if (movement_is_direction_key(normalized)) {
            if (!history_push(&history, &state)) {
                fputs("Unable to allocate undo history.\n", stderr);
                exit_code = EXIT_FAILURE;
                break;
            }

            movement_apply_player_direction(&state, normalized);
            game_update_status(&state);
            if (state.status == GAME_RUNNING) {
                movement_advance_pursuer(&state, random_source_between(0, 7));
                game_update_status(&state);
            }
        } else {
            notice = "Ignored key. Use W/A/S/D, U, or Q.";
        }
    }

    renderer_draw(&state, history_size(&history), argv[1], notice);
    history_clear(&history);
    return exit_code;
}

