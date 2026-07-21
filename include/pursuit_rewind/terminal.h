#ifndef PURSUIT_REWIND_TERMINAL_H
#define PURSUIT_REWIND_TERMINAL_H

#include <stdbool.h>

/**
 * Reads one byte immediately and restores terminal settings before returning.
 * Piped input remains supported for smoke checks.
 */
bool terminal_read_key(char *key);

#endif
