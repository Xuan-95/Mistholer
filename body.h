#ifndef BODY_H
#define BODY_H

#include "aabb.h"
#include "circle.h"
#include "vector.h"
#include <stdint.h>

typedef struct {
    double density;
    double restitution;
} BodyMaterial;

typedef struct {
    double mass;
    double invMass;
} MassData;

typedef enum { SHAPE_CIRCLE, SHAPE_AABB } ShapeType;

typedef struct {
    ShapeType type;
    union {
        Circle circle;
        AABB aabb;
    } as;
} Shape;

typedef struct {
    Shape *shape;
    BodyMaterial material;
    MassData massData;
    Vector2D position;
    Vector2D velocity;
    Vector2D force;
    double gravityScale;
    uint32_t layer;     // Layer where the body is on
    uint32_t layerMask; // Layer that interacts with the body
} Body;

void initShape(Shape* shape, ShapeType type, ...);
void initBody(Body *body, Shape *shape, Vector2D *position, Vector2D *velocity, BodyMaterial *material, uint32_t layer);
void freeBody(Body *body);

void initMaterial(BodyMaterial *material);

#endif // !BODY_H
#define BODY_H
