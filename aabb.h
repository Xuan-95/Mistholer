#ifndef AABB_H
#define AABB_H

#include "vector.h"

typedef struct {
    double halfHeight;
    double halfWidth;
} AABB;

void initAABB(AABB *aabb, double halfHeight, double halfWidth);

#endif
