#ifndef COLLISION_H
#define COLLISION_H

#include "body.h"

// A manifold is an object that contains all the information needed to solve a collision
typedef struct {
    Body *A;
    Body *B;
    float penetration;
    Vector2D normal;
} Manifold;

void initManifold(Manifold *m, Body *A, Body *B);

void resolveCollision(Manifold *m);
void positionalCorrection(Manifold *m);

// Collision detection functions
int CircleVsCircle(Manifold *m);
int AABBVsAABB(Manifold *m);
int AABBVsCircle(Manifold *m);

#endif // !COLLISION_H
