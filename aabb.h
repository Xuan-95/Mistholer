#ifndef AABB_H
#define AABB_H

#include "common.h"

typedef struct {
    double halfHeight;
    double halfWidth;
} AABB;

void initAABB(AABB *aabb, double halfHeight, double halfWidth);

#endif
