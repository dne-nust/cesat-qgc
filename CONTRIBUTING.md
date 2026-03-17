# Contributing to cesat-qgc

## Additive Policy

CESAT QGC extensions are **strictly additive**. Do not modify upstream QGC files.

- New C++ classes go in `src/CesatUI/`
- New QML files go in `src/CesatUI/`
- New CMake entries go in `custom/custom.cmake`
- Use `CESAT` prefix for all new classes, files, and QML IDs

If an upstream change is unavoidable, add a comment `// CESAT PATCH: <reason>` and register it.

## MAVLink Dialect

The CESAT dialect (`mavlink/cesat_dialect/cesat.xml`) is a submodule pointing to the same XML used in cesat-px4autopilot. Do not edit the XML here — edit it in cesat-px4autopilot and pull the submodule update.

## UI Guidelines

- CESAT panels must not break upstream QGC UI when CESAT messages are absent.
- All CESAT message handlers must gracefully handle message absence (e.g., vehicle connected but firmware not CESAT).
- Use QGC's existing MAVLink message subscription API — do not bypass it.

## PR Checklist

- [ ] No upstream QGC files modified (or `// CESAT PATCH:` comment added)
- [ ] Build passes on Ubuntu 22.04 with Qt 6.x
- [ ] CESAT panels work when CESAT messages are absent (graceful degradation)
- [ ] MAVLink message IDs are in range 60001–60004
- [ ] QML type IDs prefixed with `Cesat`
