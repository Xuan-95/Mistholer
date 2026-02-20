#include "body.h"
#include "aabb.h"
#include "circle.h"
#include "raylib.h"
#include "vector.h"
#include <stdarg.h>
#include <stdio.h>

// TODO: develop pairs generation to pass from the broad phase to the narrow phase
//
//
//

void initShape(Shape *shape, ShapeType type, ...) {
    shape->type = type;
    va_list args;
    va_start(args, type);

    switch (type) {
    case (SHAPE_CIRCLE): {
        float r = va_arg(args, double);
        initCircle(&shape->as.circle, r);
        break;
    }
    case (SHAPE_AABB): {
        double halfWidth = va_arg(args, double);
        double halfHeight = va_arg(args, double);
        initAABB(&shape->as.aabb, halfHeight, halfWidth);
        break;
    }
    }
    va_end(args);
}

void initMaterial(BodyMaterial *material) {
    material->density = 1.0;
    material->restitution = 1.0;
}

void initBody(Body *body, Shape *shape, Vector2D *position, Vector2D *velocity, BodyMaterial *material,
              uint32_t layer) {
    body->position = position ? *position : (Vector2D){0.0, 0.0};
    body->velocity = velocity ? *velocity : (Vector2D){0.0, 0.0};
    if (material) {
        body->material = *material;
    } else {
        initMaterial(&body->material);
    }
    body->gravityScale = 1.0;
    body->layer = layer;
    body->layerMask = 0xFFFFFFFF;

    body->shape = shape;
    if (shape) {
        float area = 0.0;
        switch (shape->type) {
        case (SHAPE_AABB): {
            double weight = shape->as.aabb.halfWidth * 2;
            double height = shape->as.aabb.halfHeight * 2;
            area = weight * height;
            break;
        }
        case (SHAPE_CIRCLE): {
            area = PI * shape->as.circle.r * shape->as.circle.r;
            break;
        }
        }
        body->massData.mass = body->material.density * area;
        body->massData.invMass = 1.0 / body->massData.mass;
    } else {
        body->massData.mass = 0.0;
        body->massData.invMass = 0.0;
    }
}

void freeBody(Body *body) {
}
