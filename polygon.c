#include "polygon.h"
#include "vector.h"
#include <stdlib.h>

void initPolygon(Polygon2D *polygon, int size) {
    polygon->count = size;
    if (size > 0) {
        polygon->points = malloc(size * sizeof(Vector2D));
    } else {
        polygon->points = NULL;
    }
}

void freePolygon2D(Polygon2D *polygon) {
    if (polygon->points) {
        free(polygon->points);
        polygon->points = NULL;
    }

    polygon->count = 0;
}
