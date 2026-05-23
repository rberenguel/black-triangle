#include <gtest/gtest.h>
#include "game.h"

TEST(GameState, AngleWrapsAround) {
    GameState g;
    g.angle = 359.0f;
    g.speed = 2.0f;
    g.tick(false, false);
    EXPECT_LT(g.angle, 360.0f);
    EXPECT_GE(g.angle, 0.0f);
}

TEST(GameState, SpeedIncreasesWithFaster) {
    GameState g;
    float before = g.speed;
    g.tick(true, false);
    EXPECT_GT(g.speed, before);
}

TEST(GameState, SpeedDecreasesWithSlower) {
    GameState g;
    float before = g.speed;
    g.tick(false, true);
    EXPECT_LT(g.speed, before);
}

TEST(GameState, VerticesAreEquidistantFromCenter) {
    GameState g;
    g.pos  = {0.0f, 0.0f};
    g.radius = 100.0f;

    for (int i = 0; i < 3; ++i) {
        float x, y;
        g.vertex(i, x, y);
        float dist = sqrtf(x * x + y * y);
        EXPECT_NEAR(dist, 100.0f, 0.001f);
    }
}

TEST(GameState, VerticesAre120DegreesApart) {
    GameState g;
    g.pos = {0.0f, 0.0f};
    g.angle = 0.0f;

    float x0, y0, x1, y1;
    g.vertex(0, x0, y0);
    g.vertex(1, x1, y1);

    float dot = x0 * x1 + y0 * y1;
    float mag = g.radius * g.radius;
    float cosAngle = dot / mag;
    EXPECT_NEAR(cosAngle, cosf(120.0f * DEG), 0.001f);
}
