#include "physics/shapes/aabb.h"

void initAABB(AABB *aabb, double halfHeight, double halfWidth){
    aabb->halfHeight = halfHeight;
    aabb->halfWidth = halfWidth;
}
