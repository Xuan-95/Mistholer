#ifndef BODY_H
#define BODY_H

#include "physics/shapes/aabb.h"
#include "physics/shapes/circle.h"
#include "core/common.h"
#include <stdbool.h>
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
        AABB   aabb;
    } as;
} Shape;

/* Intermediate description of the body. This struct links the frontend to the Body struct, that will be used
 * during the actual simulation */
typedef struct {
    Shape        shape;
    Vector2D     position;
    Vector2D     velocity;
    BodyMaterial material;
    double       gravityScale;
    bool         is_static;
} BodyDesc;

typedef struct {
    Shape        shape;
    BodyMaterial material;
    MassData     massData;
    Vector2D     position;
    Vector2D     velocity;
    Vector2D     force;
    double       gravityScale;
} Body;

void initShape(Shape *shape, ShapeType type, ...);
bool initBody(Body *body, const BodyDesc *desc);
void initBodyDesc(BodyDesc *desc);
void freeBody(Body *body);

void initMaterial(BodyMaterial *material);

#endif // !BODY_H
#define BODY_H
