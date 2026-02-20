#ifndef AABB_H
#define AABB_H

#include "vector.h"

typedef struct {
    Vector2D min;
    Vector2D max;
} AABB;

void initAABB(AABB *aabb, double xMin, double xMax, double yMin, double yMax);
int AABBvsAABB(AABB a, AABB b);

#endif
