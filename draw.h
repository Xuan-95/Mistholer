
#ifndef DRAW_H
#define DRAW_H

#include "aabb.h"
#include "circle.h"

void drawAABB(AABB *aabb);
void drawAABBFilled(AABB *aabb);
void drawCircle(Circle *circle);
void drawCircleFilled(Circle *circle);

#endif // !DRAW_H
