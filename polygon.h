#ifndef POLYGON_H
#define POLYGON_H

#include "vector.h"

typedef struct {
    Vector2D *points;
    int count;
} Polygon2D;

void initPolygon(Polygon2D *polygon, int size);
void freePolygon2D(Polygon2D *polygon);

#endif
