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

/** Initializes an empty history without allocating memory. */
void history_initialize(TurnHistory *history);

/** Captures both actors before a turn; the history owns the new node. */
bool history_push(TurnHistory *history, const GameState *state);

/** Restores and frees the newest snapshot, returning false when empty. */
bool history_restore_previous(TurnHistory *history, GameState *state);

/** Returns the number of snapshots currently available to undo. */
size_t history_size(const TurnHistory *history);

/** Frees every owned snapshot and resets the history to empty. */
void history_clear(TurnHistory *history);

#endif
