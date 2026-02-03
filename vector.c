#include "vector.h"

Vector2D sum2D(Vector2D a, Vector2D b) { return (Vector2D){.x = a.x + b.x, .y = a.y + b.y}; };
Vector2D diff2D(Vector2D a, Vector2D b) { return (Vector2D){.x = a.x - b.x, .y = a.y - b.y}; };
Vector2D scalarMultiply(Vector2D a, float scalar) {
    return (Vector2D){.x = a.x * scalar, .y = a.y * scalar};
}

double dot2D(Vector2D a, Vector2D b) { return (a.x * b.x) + (a.y * b.y); };

// This is a stretch. Cross product is not defined in 2D, This is the
// determinant of the matrix [[a_x,b_x],[a_y,b_y]]
double crossProduct2D(Vector2D a, Vector2D b) { return (a.x * b.y) - (a.y * b.x); }
