#include "draw.h"
#include "raylib.h"

void drawAABB(AABB *aabb) {
    DrawRectangleLines((int)aabb->min.x, (int)aabb->min.y, (int)(aabb->max.x - aabb->min.x),
                       (int)(aabb->max.y - aabb->min.y), BLUE);
}

void drawAABBFilled(AABB *aabb) {
    DrawRectangle((int)aabb->min.x, (int)aabb->min.y, (int)(aabb->max.x - aabb->min.x),
                  (int)(aabb->max.y - aabb->min.y), BLUE);
}

void drawCircle(Circle *circle) {
    DrawCircleLines((int)circle->center.x, (int)circle->center.y, circle->r, BLUE);
}

void drawCircleFilled(Circle *circle) {
    Vector2 vec = (Vector2){.x = circle->center.x, .y = circle->center.y};
    DrawCircleV(vec, circle->r, BLUE);
}
