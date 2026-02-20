#include "body.h"
#include "draw.h"
#include "raylib.h"
#include "scene.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Get current time in seconds
double getCurrentTime(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

// TODO: placeholder
void updatePhysics(float dt) {
    return;
}

void renderEngine(Scene *scene) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < scene->count; i++) {
        switch (scene->bodies[i].shape->type) {
        case (SHAPE_AABB): {
            drawAABB(&scene->bodies[i].shape->as.aabb);
            break;
        }
        case (SHAPE_CIRCLE): {
            drawCircle(&scene->bodies[i].shape->as.circle);
            break;
        }
        }
    }
    EndDrawing();
}

// only for demo purposes
void initBodies(Scene *scene) {
    // Alloca i body dinamicamente o usa addBody che li copia

    Vector2D pos_1 = {200, 100};
    Vector2D pos_2 = {400, 200};

    Body *body_1 = malloc(sizeof(Body));
    Shape *aabb = malloc(sizeof(Shape));
    initShape(aabb, SHAPE_AABB, 20.0, 100.0, 20.0, 100.0);
    initBody(body_1, aabb, &pos_1, NULL, NULL, 0);

    Body *body_2 = malloc(sizeof(Body));
    Shape *circle = malloc(sizeof(Shape));
    initShape(circle, SHAPE_CIRCLE, 30.0, pos_1);
    initBody(body_2, circle, &pos_2, NULL, NULL, 0);

    addBody(scene, body_1);
    addBody(scene, body_2);
}
int main(void) {
    // TODO: move to a global set of variables
    const float FPS = 60;
    const float dt = 1.0 / FPS;
    float accumulator = 0;

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Mistholer");
    SetTargetFPS(FPS);

    Scene scene;
    initScene(&scene);
    initBodies(&scene);
    double frameStart = getCurrentTime();

    while (!WindowShouldClose()) {
        float currentTime = getCurrentTime();

        accumulator += currentTime - frameStart;
        frameStart = currentTime;

        // Clamp the accumulator to a threshold to avoid too many physics updates
        if (accumulator > 0.2) {
            accumulator = 0.2;
        }

        if (accumulator > dt) {
            updatePhysics(dt);
            accumulator -= dt;
        }
        renderEngine(&scene);
    }

    return 0;
}
