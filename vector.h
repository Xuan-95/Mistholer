#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double x;
    double y;
} Vector2D;

Vector2D sum2D(Vector2D a, Vector2D b);
Vector2D diff2D(Vector2D a, Vector2D b);
Vector2D scalarMultiply(Vector2D a, float scalar);
double dot2D(Vector2D a, Vector2D b);
double crossProduct2D(Vector2D a, Vector2D b);

#endif
