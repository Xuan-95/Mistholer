#include "aabb.h"
#include "vector.h"

void initAABB(AABB *aabb, double halfHeight, double halfWidth){
    aabb->halfHeight = halfHeight;
    aabb->halfWidth = halfWidth;
}

