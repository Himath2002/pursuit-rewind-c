# Contributing

Pursuit Rewind is intentionally focused on map validation, reversible state,
and terminal interaction.

## Workflow

1. Create a focused branch from main.
2. Keep file I/O and terminal I/O outside the game-rule modules.
3. Add a fixture for every new map validation rule.
4. Run make clean all check.
5. Open a pull request with behavior and verification notes.

All C code must compile as C11 with strict warnings enabled.
