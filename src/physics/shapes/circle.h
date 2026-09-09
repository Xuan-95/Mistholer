#ifndef CIRCLE_H
#define CIRCLE_H

#include "core/common.h"

typedef struct {
    float r;
} Circle;

void initCircle(Circle *circle, float r);
// int CirclevsCircle(Circle a, Circle b);

#endif
