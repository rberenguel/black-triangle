#include "raylib.h"
#include "game.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#endif

#ifdef SCREEN_ROTATE
#define GAME_W SCREEN_HEIGHT
#define GAME_H SCREEN_WIDTH
static RenderTexture2D _rt;
#else
#define GAME_W SCREEN_WIDTH
#define GAME_H SCREEN_HEIGHT
#endif

static GameState g;

static int active_gamepad() {
    for (int i = 0; i < 4; i++)
        if (IsGamepadAvailable(i)) return i;
    return -1;
}

static void render_game() {
    int gp = active_gamepad();
    bool faster  = IsKeyDown(KEY_UP)       || (gp >= 0 && IsGamepadButtonDown(gp, GAMEPAD_BUTTON_LEFT_FACE_UP));
    bool slower  = IsKeyDown(KEY_DOWN)     || (gp >= 0 && IsGamepadButtonDown(gp, GAMEPAD_BUTTON_LEFT_FACE_DOWN));
    bool recolor = IsKeyPressed(KEY_SPACE) || (gp >= 0 && IsGamepadButtonPressed(gp, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
    g.tick(faster, slower);

    if (recolor) {
        g.color = { (unsigned char)GetRandomValue(80, 255),
                    (unsigned char)GetRandomValue(80, 255),
                    (unsigned char)GetRandomValue(80, 255) };
    }

    float x0, y0, x1, y1, x2, y2;
    g.vertex(0, x0, y0);
    g.vertex(1, x1, y1);
    g.vertex(2, x2, y2);

    Color c = { g.color.r, g.color.g, g.color.b, 255 };

    ClearBackground(BLACK);
    DrawTriangle({x0, y0}, {x2, y2}, {x1, y1}, c);  // CCW winding
    DrawText("SPACE: recolor   UP/DOWN: speed   ESC: quit", 10, 10, 18, DARKGRAY);
    int gp_dbg = active_gamepad();
    DrawText(gp_dbg >= 0 ? TextFormat("gamepad: %s", GetGamepadName(gp_dbg)) : "no gamepad",
             10, 34, 18, DARKGRAY);
}

static void update_draw_frame() {
#ifdef SCREEN_ROTATE
    BeginTextureMode(_rt);
    render_game();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        _rt.texture,
        { 0.0f, 0.0f, (float)GAME_W, -(float)GAME_H },
        { (float)SCREEN_WIDTH / 2.0f, (float)SCREEN_HEIGHT / 2.0f, (float)GAME_W, (float)GAME_H },
        { (float)GAME_W / 2.0f, (float)GAME_H / 2.0f },
        -90.0f,
        WHITE
    );
    EndDrawing();
#else
    BeginDrawing();
    render_game();
    EndDrawing();
#endif
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "black triangle");
    SetTargetFPS(60);

    g.pos = { (float)GAME_W / 2.0f, (float)GAME_H / 2.0f };

#ifdef SCREEN_ROTATE
    _rt = LoadRenderTexture(GAME_W, GAME_H);
#endif

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update_draw_frame, 0, 1);
#else
    while (!WindowShouldClose()) update_draw_frame();
    CloseWindow();
#endif
    return 0;
}
