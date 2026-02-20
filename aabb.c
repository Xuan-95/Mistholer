#include "aabb.h"
#include "vector.h"

void initAABB(AABB *aabb, double xMin, double xMax, double yMin, double yMax) {
    aabb->min = (Vector2D){.x = xMin, .y = yMin};
    aabb->max = (Vector2D){.x = xMax, .y = yMax};
}

// Check if two AABBs intersect, if they do not collide, return 0, otherwise return 1
int AABBvsAABB(AABB a, AABB b) {
    if ((a.max.x < b.min.x) || (a.min.x > b.max.x))
        return 0;
    if ((a.max.y < b.min.y) || (a.min.y > b.max.y))
        return 0;

    // The AABBs do not collide
    return 1;
}
