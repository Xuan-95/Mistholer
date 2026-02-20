#include "draw.h"
#include "raylib.h"

void drawAABB(AABB *aabb, Vector2D *position) {
    int x = (int)(position->x - aabb->halfWidth);
    int y = (int)(position->y - aabb->halfHeight);
    int w = (int)(aabb->halfWidth * 2);
    int h = (int)(aabb->halfHeight * 2);
    DrawRectangleLines(x, y, w, h, BLUE);
}

void drawAABBFilled(AABB *aabb, Vector2D *position) {
    int x = (int)(position->x - aabb->halfWidth);
    int y = (int)(position->y - aabb->halfHeight);
    int w = (int)(aabb->halfWidth * 2);
    int h = (int)(aabb->halfHeight * 2);
    DrawRectangle(x, y, w, h, BLUE);
}

void drawCircle(Circle *circle, Vector2D *position) {
    DrawCircleLines((int)position->x, (int)position->y, circle->r, BLUE);
}

void drawCircleFilled(Circle *circle, Vector2D *position) {
    Vector2 vec = (Vector2){.x = position->x, .y = position->y};
    DrawCircleV(vec, circle->r, BLUE);
}
