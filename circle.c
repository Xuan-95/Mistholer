#include "circle.h"
#include "vector.h"
#include <math.h>

void initCircle(Circle *circle, double cx, double cy, float r) {
    circle->r = r;
    circle->center = (Vector2D){.x = cx, .y = cy};
}

// check if two Circles intersect, if they collide return 1, otherwise return 0
int CirclevsCircle(Circle a, Circle b) {
    float r = a.r * a.r;
    r *= r;
    return r < pow(a.center.x + b.center.x, 2) + pow(a.center.y + b.center.y, 2);
}
