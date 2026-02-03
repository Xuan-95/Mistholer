#include "aabb.h"

// Check if two AABBs intersect, if they do not collide, return 0, otherwise return 1
int AABBvsAABB(AABB a, AABB b) {
    if ((a.max.x < b.min.x) || (a.min.x > b.max.x))
        return 0;
    if ((a.max.y < b.min.y) || (a.min.y > b.max.y))
        return 0;

    // The AABBs do not collide
    return 1;
}
