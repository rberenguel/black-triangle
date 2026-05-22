# black-triangle

Raylib pipeline proof-of-concept: rotating triangle, key input, three build targets.

## Prerequisites

### Mac (native)
```bash
brew install cmake
```
Raylib is fetched automatically via CMake FetchContent on first configure.

### WASM
```bash
brew install emscripten
```

### Miyoo A30 (cross-compile)
```bash
brew tap messense/macos-cross-toolchains
brew install arm-unknown-linux-gnueabihf
```

## Building

```bash
task mac        # build and run natively
task web        # build for WASM
task serve      # serve WASM build at http://localhost:8080/black_triangle.html
task miyoo      # cross-compile for Miyoo A30 / Mini Plus (Linux DRM backend)
task clean      # remove all build directories
```

## Controls

| Key   | Action        |
|-------|---------------|
| SPACE | randomise color |
| UP    | increase speed  |
| DOWN  | decrease speed  |
| ESC   | quit            |
