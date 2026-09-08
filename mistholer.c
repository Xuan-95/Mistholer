#include "body.h"
#include "collision.h"
#include "common.h"
#include "config.h"
#include "draw.h"
#include "input.h"
#include "loader.h"
#include "scene.h"
#include "ui.h"
#include <time.h>

// Get current time in seconds
double getCurrentTime(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

void resetForces(Scene *scene) {
    for (int i = 0; i < scene->count; i++) {
        scene->bodies[i].force.x = 0.0;
        scene->bodies[i].force.y = 0.0;
    }
}
// TODO: Update Manifold evaluation: now we consider all couples of bodiess (n^2), we need to implement a smarter
// evaluation of possible collisions
void evalCollisions(Scene *scene) {
    for (int i = 0; i < scene->count; i++) {
        for (int j = i + 1; j < scene->count; j++) {
            Body    *A = &scene->bodies[i];
            Body    *B = &scene->bodies[j];
            Manifold manifold;
            initManifold(&manifold, A, B);
            int isColliding = 0;
            if (A->shape.type == SHAPE_CIRCLE && B->shape.type == SHAPE_CIRCLE) {
                isColliding = CircleVsCircle(&manifold);
            } else if (A->shape.type == SHAPE_AABB && B->shape.type == SHAPE_AABB) {
                isColliding = AABBVsAABB(&manifold);
            } else if (A->shape.type == SHAPE_CIRCLE && B->shape.type == SHAPE_AABB) {
                Body *temp  = manifold.A;
                manifold.A  = manifold.B;
                manifold.B  = temp;
                isColliding = AABBVsCircle(&manifold);
            } else if (A->shape.type == SHAPE_AABB && B->shape.type == SHAPE_CIRCLE) {
                isColliding = AABBVsCircle(&manifold);
            }
            if (isColliding) {
                resolveCollision(&manifold);
                positionalCorrection(&manifold);
            }
        }
    }
}

void evalGravity(Scene *scene) {
    for (int i = 0; i < scene->count; i++) {
        Body *body     = &scene->bodies[i];
        body->force.y += (body->gravityScale * scene->gravity) * body->massData.mass;
    }
}

void updateCinematics(double dt, Scene *scene) {
    for (int i = 0; i < scene->count; i++) {
        Body *body     = &scene->bodies[i];
        body->velocity = sum2D(body->velocity, scalarMultiply(scalarMultiply(body->force, body->massData.invMass), dt));
        body->position = sum2D(body->position, scalarMultiply(body->velocity, dt));
    }
}

void updatePhysics(float dt, Scene *scene) {
    resetForces(scene);
    evalGravity(scene);
    evalCollisions(scene);
    updateCinematics(dt, scene);
}

void renderEngine(Scene *scene, UIState *ui, double frameTime) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < scene->count; i++) {
        switch (scene->bodies[i].shape.type) {
        case (SHAPE_AABB): {
            drawAABBFilled(&scene->bodies[i].shape.as.aabb, &scene->bodies[i].position);
            break;
        }
        case (SHAPE_CIRCLE): {
            drawCircleFilled(&scene->bodies[i].shape.as.circle, &scene->bodies[i].position);
            break;
        }
        }
    }
    DrawRectangle(5, 5, 220, 55, BLACK);
    DrawText(TextFormat("FrameTime: %.2f ms", frameTime * 1000), 10, 10, 20, WHITE);
    DrawText(TextFormat("Bodies: %d", scene->count), 10, 35, 20, WHITE);
    drawUI(ui);
    EndDrawing();
}

int main(void) {
    // Default config
    Config config = {800, 600, 30};
    parseConfig("./config.ini", &config);

    const float dt          = 1.0 / config.FPS;
    float       accumulator = 0;

    InitWindow(config.WINDOW_WIDTH, config.WINDOW_HEIGHT, "Mistholer");
    SetTargetFPS(config.FPS);

    const int   screenWidth  = config.WINDOW_WIDTH;
    const int   screenHeight = config.WINDOW_HEIGHT;
    const float FPS          = config.FPS;
    printf("###########\n");
    printf("Configuration:\n");
    printf("FPS: %f\nSCREEN WIDTH: %d\nSCREEN HEIGHT: %d\n", FPS, screenWidth, screenHeight);
    printf("###########\n");

    UIState ui = {.selectedShape = SHAPE_CIRCLE};

    Scene   scene;
    initScene(&scene);
    loadSceneFromJson("test.json", &scene);
    double frameStart = getCurrentTime();
    double updateTime = 0.0;

    while (!WindowShouldClose()) {
        float currentTime = getCurrentTime();

        accumulator += currentTime - frameStart;
        frameStart   = currentTime;

        // Clamp the accumulator to a threshold to avoid too many physics updates
        if (accumulator > 0.2) {
            accumulator = 0.2;
        }
        double workStart = getCurrentTime();

        if (accumulator > dt) {
            updatePhysics(dt, &scene);
            accumulator -= dt;
        }
        handleInput(&scene, &ui);
        updateTime = getCurrentTime() - workStart;
        renderEngine(&scene, &ui, updateTime);
    }

    return 0;
}
