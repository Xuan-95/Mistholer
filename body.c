#include "body.h"
#include "raylib.h"
#include <stdarg.h>

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
        double halfWidth  = va_arg(args, double);
        double halfHeight = va_arg(args, double);
        initAABB(&shape->as.aabb, halfHeight, halfWidth);
        break;
    }
    }
    va_end(args);
}

void initMaterial(BodyMaterial *material) {
    material->density     = 1.0;
    material->restitution = 1.0;
}

bool initBody(Body *body, const BodyDesc *desc) {
    Body result = {0};

    result.shape        = desc->shape;
    result.position     = desc->position;
    result.velocity     = desc->velocity;
    result.material     = desc->material;
    result.gravityScale = desc->gravityScale;

    if (desc->is_static) {
        result.massData.mass    = 0.0;
        result.massData.invMass = 0.0;
        result.velocity         = (Vector2D){0.0, 0.0};
    } else {
        double area;

        switch (desc->shape.type) {
        case SHAPE_AABB:
            area = 4.0 * desc->shape.as.aabb.halfWidth * desc->shape.as.aabb.halfHeight;
            break;

        case SHAPE_CIRCLE:
            area = PI * desc->shape.as.circle.r * desc->shape.as.circle.r;
            break;

        default:
            return false;
        }

        double mass = result.material.density * area;
        if (!isfinite(mass) || mass <= 0.0)
            return false;

        double invMass = 1.0 / mass;
        if (!isfinite(invMass))
            return false;

        result.massData.mass    = mass;
        result.massData.invMass = invMass;
    }

    *body = result;
    return true;
}
void freeBody(Body *body) {}
