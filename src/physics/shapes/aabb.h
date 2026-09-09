#ifndef AABB_H
#define AABB_H

#include "core/common.h"

typedef struct {
    double halfHeight;
    double halfWidth;
} AABB;

void initAABB(AABB *aabb, double halfHeight, double halfWidth);

#endif
