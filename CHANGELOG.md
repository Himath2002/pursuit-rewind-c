# Changelog

Notable changes to Pursuit Rewind are recorded here. The project follows
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2026-07-21

### Added

- Validated map-driven terminal sessions.
- Toroidal player and pursuer movement.
- Paired player/pursuer snapshots with multi-level undo.
- Immediate target victory and protected target-cell pursuit rules.
- Strict C11 build and focused verification suite.
- Ubuntu and macOS continuous integration.
- Project-specific architecture and gameplay visuals.

### Changed

- Replaced the generic O(n) tail-removal list with an O(1) history stack.
- Replaced duplicated character-grid state with derived rendering.
- Reimplemented random and terminal utilities behind narrow interfaces.
- Added bounded integer tokenization and actionable failures for malformed map
  input.
- Made adjacent pursuit honor toroidal proximity across map seams.

[Unreleased]: https://github.com/Himath2002/pursuit-rewind-c/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/Himath2002/pursuit-rewind-c/releases/tag/v1.0.0
