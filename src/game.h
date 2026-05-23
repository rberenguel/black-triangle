#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static constexpr float DEG = M_PI / 180.0f;

struct Vec2 { float x, y; };
struct RGB  { unsigned char r, g, b; };

struct GameState {
    float angle  = 0.0f;
    float speed  = 1.5f;
    RGB   color  = {255, 255, 255};
    Vec2  pos    = {320.0f, 240.0f};
    float radius = 80.0f;

    void tick(bool faster, bool slower) {
        if (faster) speed += 0.05f;
        if (slower) speed -= 0.05f;
        angle += speed;
        if (angle >= 360.0f) angle -= 360.0f;
        if (angle <    0.0f) angle += 360.0f;
    }

    void vertex(int index, float& x, float& y) const {
        float a = (angle + index * 120.0f) * DEG;
        x = pos.x + radius * cosf(a);
        y = pos.y + radius * sinf(a);
    }
};
