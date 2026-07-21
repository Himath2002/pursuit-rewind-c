#include "pursuit_rewind/game.h"
#include "pursuit_rewind/history.h"
#include "pursuit_rewind/map_loader.h"
#include "pursuit_rewind/movement.h"

#include <stdio.h>
#include <stdlib.h>

static int failures;

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            failures++; \
        } \
    } while (0)

static GameState new_state(void)
{
    GameState state = {
        .rows = 5,
        .columns = 7,
        .player = {1, 1},
        .pursuer = {5, 7},
        .target = {3, 4},
        .status = GAME_RUNNING
    };
    return state;
}

static void test_map_loader_accepts_classic_map(void)
{
    GameState state;
    MapLoadResult result = map_loader_read("maps/classic.map", &state);

    EXPECT_TRUE(result == MAP_LOAD_OK);
    EXPECT_TRUE(state.rows == 7);
    EXPECT_TRUE(state.columns == 9);
    EXPECT_TRUE(game_positions_equal(state.player, (Position){2, 2}));
    EXPECT_TRUE(game_positions_equal(state.target, (Position){6, 7}));
    EXPECT_TRUE(game_positions_equal(state.pursuer, (Position){4, 5}));
}

static void test_map_loader_rejects_invalid_maps(void)
{
    GameState state;

    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/invalid-dimensions.map",
            &state
    ) == MAP_LOAD_INVALID_DIMENSIONS);
    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/duplicate-player.map",
            &state
    ) == MAP_LOAD_DUPLICATE_ACTOR);
    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/truncated.map",
            &state
    ) == MAP_LOAD_TRUNCATED);
    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/invalid-cell.map",
            &state
    ) == MAP_LOAD_INVALID_CELL);
    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/invalid-number.map",
            &state
    ) == MAP_LOAD_INVALID_CELL);
    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/missing-target.map",
            &state
    ) == MAP_LOAD_MISSING_ACTOR);
    EXPECT_TRUE(map_loader_read(
            "tests/fixtures/trailing-data.map",
            &state
    ) == MAP_LOAD_TRAILING_DATA);
}

static void test_player_wraps_across_both_axes(void)
{
    GameState state = new_state();

    movement_apply_player_direction(&state, 'w');
    movement_apply_player_direction(&state, 'a');

    EXPECT_TRUE(game_positions_equal(state.player, (Position){5, 7}));
}

static void test_distant_pursuer_wraps(void)
{
    GameState state = new_state();
    state.player = (Position){3, 3};
    state.pursuer = (Position){1, 1};

    movement_advance_pursuer(&state, 4);

    EXPECT_TRUE(game_positions_equal(state.pursuer, (Position){5, 7}));
}

static void test_pursuer_protects_target_cell(void)
{
    GameState state = new_state();
    state.player = (Position){5, 7};
    state.pursuer = (Position){3, 3};
    state.target = (Position){3, 4};

    movement_advance_pursuer(&state, 3);

    EXPECT_TRUE(game_positions_equal(state.pursuer, (Position){3, 3}));
}

static void test_adjacent_pursuer_chases(void)
{
    GameState state = new_state();
    state.player = (Position){2, 2};
    state.pursuer = (Position){3, 3};

    movement_advance_pursuer(&state, 7);
    game_update_status(&state);

    EXPECT_TRUE(game_positions_equal(state.player, state.pursuer));
    EXPECT_TRUE(state.status == GAME_PLAYER_CAUGHT);
}

static void test_adjacent_pursuer_chases_across_wrap_boundary(void)
{
    GameState state = new_state();
    state.player = (Position){1, 4};
    state.pursuer = (Position){5, 4};

    movement_advance_pursuer(&state, 3);
    game_update_status(&state);

    EXPECT_TRUE(game_positions_equal(state.player, state.pursuer));
    EXPECT_TRUE(state.status == GAME_PLAYER_CAUGHT);
}

static void test_history_restores_in_lifo_order(void)
{
    GameState state = new_state();
    TurnHistory history;

    history_initialize(&history);
    EXPECT_TRUE(history_push(&history, &state));
    state.player = (Position){1, 2};
    state.pursuer = (Position){4, 7};
    EXPECT_TRUE(history_push(&history, &state));
    state.player = (Position){1, 3};
    state.pursuer = (Position){4, 6};

    EXPECT_TRUE(history_size(&history) == 2U);
    EXPECT_TRUE(history_restore_previous(&history, &state));
    EXPECT_TRUE(game_positions_equal(state.player, (Position){1, 2}));
    EXPECT_TRUE(game_positions_equal(state.pursuer, (Position){4, 7}));
    EXPECT_TRUE(history_restore_previous(&history, &state));
    EXPECT_TRUE(game_positions_equal(state.player, (Position){1, 1}));
    EXPECT_TRUE(game_positions_equal(state.pursuer, (Position){5, 7}));
    EXPECT_TRUE(!history_restore_previous(&history, &state));

    history_clear(&history);
}

static void test_target_arrival_has_priority(void)
{
    GameState state = new_state();
    state.player = state.target;
    state.pursuer = state.target;

    game_update_status(&state);

    EXPECT_TRUE(state.status == GAME_PLAYER_WON);
}

int main(void)
{
    test_map_loader_accepts_classic_map();
    test_map_loader_rejects_invalid_maps();
    test_player_wraps_across_both_axes();
    test_distant_pursuer_wraps();
    test_pursuer_protects_target_cell();
    test_adjacent_pursuer_chases();
    test_adjacent_pursuer_chases_across_wrap_boundary();
    test_history_restores_in_lifo_order();
    test_target_arrival_has_priority();

    if (failures != 0) {
        fprintf(stderr, "%d test expectation(s) failed.\n", failures);
        return EXIT_FAILURE;
    }

    puts("All Pursuit Rewind checks passed.");
    return EXIT_SUCCESS;
}
