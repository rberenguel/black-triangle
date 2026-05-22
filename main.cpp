#include "raylib.h"
#include <cmath>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

static float angle   = 0.0f;
static float speed   = 1.5f;
static Color color   = WHITE;
static Vector2 pos;
static float radius  = 80.0f;

static void update_draw_frame() {
    if (IsKeyPressed(KEY_SPACE))
        color = { (unsigned char)GetRandomValue(80, 255),
                  (unsigned char)GetRandomValue(80, 255),
                  (unsigned char)GetRandomValue(80, 255), 255 };
    if (IsKeyDown(KEY_UP))   speed += 0.05f;
    if (IsKeyDown(KEY_DOWN)) speed -= 0.05f;

    angle += speed;

    float cx = pos.x, cy = pos.y;
    Vector2 v1 = { cx + radius * cosf(angle * DEG2RAD),
                   cy + radius * sinf(angle * DEG2RAD) };
    Vector2 v2 = { cx + radius * cosf((angle + 120) * DEG2RAD),
                   cy + radius * sinf((angle + 120) * DEG2RAD) };
    Vector2 v3 = { cx + radius * cosf((angle + 240) * DEG2RAD),
                   cy + radius * sinf((angle + 240) * DEG2RAD) };

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTriangle(v1, v3, v2, color);
        DrawText("SPACE: recolor   UP/DOWN: speed   ESC: quit", 10, 10, 18, DARKGRAY);
    EndDrawing();
}

int main() {
    InitWindow(640, 480, "black triangle");
    SetTargetFPS(60);
    pos = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update_draw_frame, 0, 1);
#else
    while (!WindowShouldClose()) update_draw_frame();
    CloseWindow();
#endif
    return 0;
}
