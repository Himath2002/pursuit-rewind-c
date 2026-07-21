#ifndef PURSUIT_REWIND_HISTORY_H
#define PURSUIT_REWIND_HISTORY_H

#include <stdbool.h>
#include <stddef.h>

#include "pursuit_rewind/game.h"

/** Actor positions captured immediately before one accepted player turn. */
typedef struct {
    Position player;
    Position pursuer;
} TurnSnapshot;

typedef struct HistoryNode HistoryNode;

/** O(1) LIFO history used by the undo command. */
typedef struct {
    HistoryNode *top;
    size_t size;
} TurnHistory;

void history_initialize(TurnHistory *history);
bool history_push(TurnHistory *history, const GameState *state);
bool history_restore_previous(TurnHistory *history, GameState *state);
size_t history_size(const TurnHistory *history);
void history_clear(TurnHistory *history);

#endif

