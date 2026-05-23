# Session Compaction Summary

## User Intent
- Build a raylib-based "black triangle" pipeline proof-of-concept as a template for porting a 32blit game (roids) to handheld devices
- Validate three build targets: Mac native, WASM (for web publishing), and Miyoo A30 (ARM Linux)
- Fix a series of environment and toolchain issues along the way (cmake arch, PATH, terminal colors)

## Contextual Work Summary

### Project Setup
- Created `/Users/ruben/black-triangle/` as a standalone template project
- Rotating triangle + SPACE/UP/DOWN input, using raylib's `DrawTriangle` (CCW winding required)
- CMakeLists uses FetchContent to pull raylib master; `CMAKE_POLICY_VERSION_MINIMUM 3.5` needed for new cmake
- `PLATFORM` must be forced into CMake cache before FetchContent so raylib picks it up

### Mac Native Build
- Fixed cmake PATH issue: Intel cmake at `/usr/local/bin` shadowed arm64 one at `/opt/homebrew/bin`
- Added `export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:$PATH"` to `/Users/ruben/code/dotfiles/zshrc.zsh` (via `.zprofile`)
- Mac build works: `cmake -B build.mac -DPLATFORM=Desktop && cmake --build build.mac -j8`

### WASM Build
- Requires `brew install emscripten`; confirmed working via `task web`
- Main loop must be extracted to a standalone function with `#ifdef PLATFORM_WEB` / `emscripten_set_main_loop`
- Output served locally with `python3 -m http.server 8080 --directory build.web`

### Miyoo A30 Cross-Compile (in progress)
- Device: Miyoo A30, Spruce OS, ARM Cortex-A53, glibc 2.23, GCC 6.4.1, SDL2 available
- Ports go in `/mnt/SDCARD/Roms/A30PORTS/`, launch script pattern from `Dino Jump.sh`
- DRM platform failed: device has no `libdrm.so.2`
- SDL platform failed: host macOS SDL2 (with framework deps) found instead of target SDL2
- Ubuntu 24.04 Docker image with arm-linux-gnueabihf worked for compile but glibc mismatch at runtime (binary required GLIBC_2.38, device has 2.23)
- **Current approach**: Dockerfile uses Linaro GCC 6.4.1 (matches device toolchain exactly, targets glibc 2.23) + host `libsdl2-dev` for headers + device's `libSDL2-2.0.so.0` for linking
- Device SDL2 scp'd to `toolchains/libs/libSDL2-2.0.so.0`; bundled with binary at deploy time
- `LD_LIBRARY_PATH` set in launch script to port directory
- **Build not yet confirmed working** — `task miyoo-image && rm -rf build.miyoo && task miyoo` was the next step

### Environment Fixes
- OrbStack installed as Docker alternative; sourced via `~/.orbstack/shell/init.zsh`
- `TERM=xterm-256color` removed from zshrc (was masking Ghostty's terminfo)
- tmux `default-terminal` changed from `screen-256color` to `tmux-256color` + Ghostty RGB override
- Ghostty theme changed from `Black Metal (Bathory)` to `Dracula` with `background = 000000`

### Repo
- Git + jj (colocated) initialized in `/Users/ruben/black-triangle/`
- Initial commit contains all 8 files; `.gitignore` covers all build dirs

## Files Touched

### Build & Docker
- **CMakeLists.txt**: FetchContent for raylib, PLATFORM cache fix, SDL2 cross-compile vars when `CMAKE_CROSSCOMPILING`
- **Dockerfile.miyoo**: Linaro GCC 6.4.1 from releases.linaro.org + host libsdl2-dev for headers
- **taskfile.yml**: tasks for mac, web, serve, miyoo-image, miyoo, deploy-miyoo, clean

### Toolchain
- **toolchains/miyoo-a30.cmake**: arm-linux-gnueabihf cross-compile settings, CPU flags for Cortex-A53
- **toolchains/libs/libSDL2-2.0.so.0**: scp'd from device (`/mnt/SDCARD/miyoo/lib/`)

### Deploy
- **deploy/spruce.sh**: Spruce launch script; sets `LD_LIBRARY_PATH` to port dir, wraps joystickinput signals

### Game
- **main.cpp**: Rotating triangle, SPACE/UP/DOWN input, `#ifdef PLATFORM_WEB` loop structure

### Dotfiles (outside project)
- **~/.zprofile** (real: `/Users/ruben/code/dotfiles/zshrc.zsh`... actually `.zprofile`): added `/opt/homebrew/bin` before `/usr/local/bin`
- **~/code/dotfiles/zshrc.zsh**: removed `TERM=xterm-256color`, added `CLICOLOR_FORCE=1`
- **~/code/dotfiles/tmux/tmux.conf**: changed `default-terminal` to `tmux-256color`, added Ghostty RGB override
- **~/.config/ghostty/config**: changed theme to Dracula + `background = 000000`
