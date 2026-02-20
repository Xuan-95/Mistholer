#ifndef DRAW_H
#define DRAW_H
#include "aabb.h"
#include "circle.h"
#include "vector.h"

void drawAABB(AABB *aabb, Vector2D *position);
void drawAABBFilled(AABB *aabb, Vector2D *position);
void drawCircle(Circle *circle, Vector2D *position);
void drawCircleFilled(Circle *circle, Vector2D *position);

#endif // !DRAW_H
