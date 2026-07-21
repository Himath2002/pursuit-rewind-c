#include "pursuit_rewind/history.h"

#include <stdlib.h>

struct HistoryNode {
    TurnSnapshot snapshot;
    struct HistoryNode *next;
};

void history_initialize(TurnHistory *history)
{
    if (history == NULL) {
        return;
    }
    history->top = NULL;
    history->size = 0U;
}

bool history_push(TurnHistory *history, const GameState *state)
{
    HistoryNode *node;

    if (history == NULL || state == NULL) {
        return false;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }

    node->snapshot.player = state->player;
    node->snapshot.pursuer = state->pursuer;
    node->next = history->top;
    history->top = node;
    history->size++;
    return true;
}

bool history_restore_previous(TurnHistory *history, GameState *state)
{
    HistoryNode *node;

    if (history == NULL || state == NULL || history->top == NULL) {
        return false;
    }

    node = history->top;
    history->top = node->next;
    history->size--;

    state->player = node->snapshot.player;
    state->pursuer = node->snapshot.pursuer;
    state->status = GAME_RUNNING;
    free(node);
    return true;
}

size_t history_size(const TurnHistory *history)
{
    return history == NULL ? 0U : history->size;
}

void history_clear(TurnHistory *history)
{
    HistoryNode *node;

    if (history == NULL) {
        return;
    }

    node = history->top;
    while (node != NULL) {
        HistoryNode *next = node->next;
        free(node);
        node = next;
    }

    history->top = NULL;
    history->size = 0U;
}

