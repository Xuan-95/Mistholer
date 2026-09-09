#ifndef DRAW_H
#define DRAW_H
#include "physics/shapes/aabb.h"
#include "physics/shapes/circle.h"
#include "core/common.h"

void drawAABB(AABB *aabb, Vector2D *position);
void drawAABBFilled(AABB *aabb, Vector2D *position);
void drawCircle(Circle *circle, Vector2D *position);
void drawCircleFilled(Circle *circle, Vector2D *position);

#endif // !DRAW_H
