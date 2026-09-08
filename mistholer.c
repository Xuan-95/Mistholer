#include "body.h"
#include "collision.h"
#include "common.h"
#include "draw.h"
#include "loader.h"
#include "raylib.h"
#include "scene.h"
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

void renderEngine(Scene *scene) {
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
    EndDrawing();
}

int main(void) {
    // TODO: move to a global set of variables
    const float FPS         = 60;
    const float dt          = 1.0 / FPS;
    float       accumulator = 0;

    const int   screenWidth  = 1920;
    const int   screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Mistholer");
    SetTargetFPS(FPS);

    Scene scene;
    initScene(&scene);
    loadSceneFromJson("test.json", &scene);
    double frameStart = getCurrentTime();

    while (!WindowShouldClose()) {
        float currentTime = getCurrentTime();

        accumulator += currentTime - frameStart;
        frameStart   = currentTime;

        // Clamp the accumulator to a threshold to avoid too many physics updates
        if (accumulator > 0.2) {
            accumulator = 0.2;
        }

        if (accumulator > dt) {
            updatePhysics(dt, &scene);
            accumulator -= dt;
        }
        renderEngine(&scene);
    }

    return 0;
}
