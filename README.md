# black-triangle

A raylib pipeline proof-of-concept and **game template** for targeting multiple platforms from a single C++ codebase. The name refers to [the black triangle](https://rampantgames.com/blog/?p=7745) — the unglamorous first milestone that proves your entire rendering pipeline works end to end. Confirmed working on:

- Mac (native, Apple Silicon)
- Browser (WASM via Emscripten)
- Miyoo A30 handheld (ARM, Spruce OS)

The demo is a rotating triangle with colour and speed controls — just enough to validate rendering, input, and the full build/deploy pipeline before starting a real game.

## Project structure

```
src/
  game.h       — pure C++ game logic, no raylib dependency (testable)
  main.cpp     — raylib entry point: rendering, input, platform loop
tests/
  test_game.cpp — Google Test suite for game logic
toolchains/
  miyoo-a30.cmake          — CMake toolchain for Linaro GCC 6.4.1
  cmake/SDL2/SDL2Config.cmake — SDL2 stub for cross-compile
  libs/libSDL2-2.0.so.0   — SDL2 from the device (bundled at deploy time)
deploy/
  spruce.sh    — Spruce OS launch script (sets LD_LIBRARY_PATH, d-pad mode)
Dockerfile.miyoo — build environment: Ubuntu 24.04 + Linaro GCC 6.4.1 + armhf SDL2 headers
```

The key template pattern: keep all game state and logic in `src/game.h` with no platform dependencies, so it can be unit-tested on Mac without any hardware or display. `src/main.cpp` is a thin raylib wrapper that calls into it.

## Prerequisites

### Mac (native)
```bash
brew install cmake go-task
```
Raylib is fetched automatically via CMake FetchContent on first configure.

### WASM
```bash
brew install emscripten
```

### Miyoo A30 (cross-compile)
Requires Docker (or OrbStack). The Docker image is self-contained — no host toolchain needed.

The device's `libSDL2-2.0.so.0` must be present at `toolchains/libs/` (not included in this repo — it is a Miyoo/Spruce custom build and not ours to redistribute). Copy it from your device once over SSH:
```bash
scp root@<device-ip>:/mnt/SDCARD/miyoo/lib/libSDL2-2.0.so.0 toolchains/libs/
```

## Building

```bash
task mac          # build and run natively on Mac
task web          # build for WASM
task serve        # serve WASM build at http://localhost:8080
task test         # run unit tests (Mac only, fast)
task miyoo-image  # build the Docker cross-compile image (once, or after Dockerfile changes)
task miyoo        # cross-compile for Miyoo A30
task deploy-miyoo # copy binary + SDL2 + launch script to mounted SD card (/Volumes/A30)
task clean        # remove all build directories
```

## Deployment (Miyoo A30)

With the SD card mounted at `/Volumes/A30`:
```bash
task deploy-miyoo
```

This copies the binary, `libSDL2-2.0.so.0`, and `deploy/spruce.sh` to the correct locations for Spruce OS. The port appears under **PORTS** in the main menu.

## Controls

| Key   | Action          |
|-------|-----------------|
| SPACE | randomise color |
| UP    | increase speed  |
| DOWN  | decrease speed  |
| ESC   | quit            |

On the Miyoo A30 the face buttons map to the arrow keys and START/SELECT via the device's default key mappings.

## Missing / next steps

Before using this as a template for a real game, these three things need to be added:

- **Gamepad button mapping** — gamepad input is wired up alongside keyboard (d-pad for speed, face button for recolor), but the exact button constants have not been verified against all target controllers. Check `GAMEPAD_BUTTON_LEFT_FACE_*` and `GAMEPAD_BUTTON_RIGHT_FACE_*` in `src/main.cpp` against your hardware. On web, click the canvas then press a button on the gamepad to activate it (browser security requirement). On Mac, raylib's backend uses IOKit HID which only sees classic Bluetooth HID devices — BLE gamepads (confirmed: IINE IG05 fails, 8BitDo Zero 2 works) are not detected. Use keyboard on Mac for dev and verify gamepad on web or device.
- **Asset loading** — any real game needs fonts, textures, and sounds. For WASM, emscripten requires `--preload-file assets/` in the link options; for Miyoo, paths must be relative to the binary. Nothing in the build handles this yet and it touches all three targets.
- **Audio** — `InitAudioDevice()` is never called. On WASM, audio also requires a user gesture (keypress or click) before the browser allows it, which needs explicit handling.

## Notes

- Miyoo A30 display is portrait (480×640) but game logic runs at landscape (640×480); the SDL layer rotates the output, so all game coordinates are landscape.
- The cross-compile uses Linaro GCC 6.4.1 to match the device's glibc 2.23. A modern host compiler produces binaries that won't run.
- SDL2 is dynamically linked and bundled with the port rather than statically linked, because the device's SDL2 contains hardware-specific patches.
- The WASM build requires `PLATFORM_WEB` / `emscripten_set_main_loop` — the `#ifdef` in `src/main.cpp` handles this.
