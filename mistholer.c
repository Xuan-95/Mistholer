#include "body.h"
#include "collision.h"
#include "draw.h"
#include "raylib.h"
#include "scene.h"
#include "stdio.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
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
        for (int j = i+1; j < scene->count; j++) {
            Body *A = &scene->bodies[i];
            Body *B = &scene->bodies[j];
            Manifold manifold;
            initManifold(&manifold, A, B);
            int isColliding = 0;
            if (A->shape->type == SHAPE_CIRCLE && B->shape->type == SHAPE_CIRCLE) {
                isColliding = CircleVsCircle(&manifold);
            } else if (A->shape->type == SHAPE_AABB && B->shape->type == SHAPE_AABB) {
                isColliding = AABBVsAABB(&manifold);
            } else if (A->shape->type == SHAPE_CIRCLE && B->shape->type == SHAPE_AABB) {
                Body *temp = manifold.A;
                manifold.A = manifold.B;
                manifold.B = temp;
                isColliding = AABBVsCircle(&manifold);
            } else if (A->shape->type == SHAPE_AABB && B->shape->type == SHAPE_CIRCLE) {
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
        Body *body = &scene->bodies[i];
        // TODO: Make g global
        body->force.y += (body->gravityScale * 300) * body->massData.mass;
    }
}

void updateCinematics(double dt, Scene *scene) {
    for (int i = 0; i < scene->count; i++) {
        Body *body = &scene->bodies[i];
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
        switch (scene->bodies[i].shape->type) {
        case (SHAPE_AABB): {
            drawAABBFilled(&scene->bodies[i].shape->as.aabb, &scene->bodies[i].position);
            break;
        }
        case (SHAPE_CIRCLE): {
            drawCircleFilled(&scene->bodies[i].shape->as.circle, &scene->bodies[i].position);
            break;
        }
        }
    }
    EndDrawing();
}

// only for demo purposes
void initBodies(Scene *scene) {
    // Alloca i body dinamicamente o usa addBody che li copia

    Vector2D pos_1 = {200, 500};
    Vector2D pos_2 = {400, 200};
    Vector2D pos_3 = {420,300};

    Vector2D ground_pos = {400, 600};
    Body *ground_body = malloc(sizeof(Body));
    Shape *ground_shape = malloc(sizeof(Shape));
    initShape(ground_shape, SHAPE_AABB, 400.0, 10.0);
    initBody(ground_body, ground_shape, &ground_pos, NULL, NULL, 0);
    ground_body->massData.mass = 0.0;
    ground_body->massData.invMass = 0.0;

    Body *body_1 = malloc(sizeof(Body));
    Shape *aabb = malloc(sizeof(Shape));
    initShape(aabb, SHAPE_AABB, 100.0, 20.0);
    initBody(body_1, aabb, &pos_1, NULL, NULL, 0);

    Body *body_2 = malloc(sizeof(Body));
    Shape *circle = malloc(sizeof(Shape));
    initShape(circle, SHAPE_CIRCLE, 30.0);
    initBody(body_2, circle, &pos_2, NULL, NULL, 0);

    Body *body_3 = malloc(sizeof(Body));
    Shape *circle_3 = malloc(sizeof(Shape));
    initShape(circle_3, SHAPE_CIRCLE, 50.0);
    initBody(body_3, circle_3, &pos_3, NULL, NULL, 0);

    addBody(scene, body_1);
    addBody(scene, body_2);
    addBody(scene, body_3);
    addBody(scene, ground_body);
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
            updatePhysics(dt, &scene);
            accumulator -= dt;
        }
        renderEngine(&scene);
    }

    return 0;
}
