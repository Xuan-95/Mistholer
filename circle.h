#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector.h"

typedef struct {
    float r;
    Vector2D center;
} Circle;

void initCircle(Circle *circle, double cx, double cy, float r);
int CirclevsCircle(Circle a, Circle b);

#endif
