#include "collision.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

void initManifold(Manifold *m, Body *A, Body *B) {
    m->A = A;
    m->B = B;
    m->penetration = 0.0;
    m->normal = (Vector2D){.x = 0.0, .y = 0.0};
}

// Solve collision between two objects, updating the velocities applying an impulse
void resolveCollision(Manifold *m) {
    Vector2D relativeVelocity = diff2D(m->B->velocity, m->A->velocity);

    double velocityAlongNormal = dot2D(relativeVelocity, m->normal);

    // If the objects are separating, do not evaluate collision
    if (velocityAlongNormal > 0)
        return;

    float e = fmin(m->A->material.restitution, m->B->material.restitution);

    float j = (-(1 + e) * velocityAlongNormal) / (m->A->massData.invMass + m->B->massData.invMass);
    Vector2D impulse = scalarMultiply(m->normal, j);

    m->A->velocity = diff2D(m->A->velocity, scalarMultiply(impulse, m->A->massData.invMass));
    m->B->velocity = sum2D(m->B->velocity, scalarMultiply(impulse, m->B->massData.invMass));
}

// This function is needed to avoid the penetration of an object inside another
// caused by floating point error. Penetration arises often when an object colliding with a static
// one (for example, the terrain)
void positionalCorrection(Manifold *m) {
    const float percent = 0.2;

    // add slop term to avoid jitter
    const float slop = 0.001;
    Vector2D correction = scalarMultiply(m->normal, fmax(m->penetration - slop, 0) /
                                                        (m->A->massData.invMass + m->B->massData.invMass) * percent);

    m->A->position = diff2D(m->A->position, scalarMultiply(correction, m->A->massData.invMass));
    m->A->position = sum2D(m->B->position, scalarMultiply(correction, m->B->massData.invMass));
}

// This function evaluates the collision between two circles, if the circles collide return 1,
// otherwise return 0 The evaluation is performed simply by calculating the distance between the
// centers
int CircleVsCircle(Manifold *m) {
    Body *A = m->A;
    Body *B = m->B;

    Vector2D ab = diff2D(B->position, A->position);
    float squaredRadius = pow(A->shape->as.circle.r + B->shape->as.circle.r, 2);

    float squaredDistance = dot2D(ab, ab);
    if (squaredDistance > squaredRadius)
        // Circles are not colliding
        return 0;

    // Circles are colliding, we need to evaluate the manifold
    float distance = sqrt(squaredDistance);
    if (distance > 0) {
        m->penetration = squaredRadius - distance;
        m->normal = scalarMultiply(ab, 1.0 / distance);
        return 1;
    } else {
        m->penetration = A->shape->as.circle.r;
        m->normal = (Vector2D){.x = 1, .y = 0};
        return 1;
    }
}

// This function evaluates the collision between two AABBs, it returns 0 if they do not collide, 1
// otherwise The collision is evauated using the Separating Axis Theorem (SAT)
int AABBVsAABB(Manifold *m) {
    Body *A = m->A;
    Body *B = m->B;
    Vector2D ab = diff2D(B->position, A->position);
    AABB abox = A->shape->as.aabb;
    AABB bbox = B->shape->as.aabb;

    // Evaluate x-axis
    float x_overlap = abox.halfWidth + bbox.halfWidth - fabs(ab.x);

    if (x_overlap > 0) {
        // Evaluate y-axis
        float y_overlap = abox.halfHeight + bbox.halfHeight - fabs(ab.y);
        if (y_overlap > 0) {
            // find the minimum overlap
            if (x_overlap < y_overlap) {
                // find the normal direction
                if (ab.x < 0) {
                    m->normal = (Vector2D){.x = -1, .y = 0};
                } else {
                    m->normal = (Vector2D){.x = 1, .y = 0};
                }
                m->penetration = x_overlap;
                return 1;

            } else {
                // find the normal direction
                if (ab.y < 0) {
                    m->normal = (Vector2D){.x = 0, .y = -1};
                } else {
                    m->normal = (Vector2D){.x = 0, .y = 1};
                }
                m->penetration = y_overlap;
                return 1;
            }
        }
    }
    return 0;
}

static inline float clamp_float(float x, float low, float high) {
    return x < low ? low : (x > high ? high : x);
}

// Same as above, the evaluation is performed evaluating the distance of the circle center with the
// closest point of the aabb
int AABBVsCircle(Manifold *m) {
    Body *A = m->A; // AABB
    Body *B = m->B; // Circle

    AABB aabb = A->shape->as.aabb;
    double aabb_xmin = A->position.x - aabb.halfWidth;
    double aabb_xmax = A->position.x + aabb.halfWidth;
    double aabb_ymin = A->position.y - aabb.halfHeight;
    double aabb_ymax = A->position.y + aabb.halfHeight;
    Circle circle = B->shape->as.circle;

    Vector2D circleCenter = B->position;
    Vector2D closest;
    closest.x = clamp_float(circleCenter.x, aabb_xmin, aabb_xmax);
    closest.y = clamp_float(circleCenter.y, aabb_ymin, aabb_ymax);
    Vector2D delta = diff2D(circleCenter, closest);
    float squaredDistance = dot2D(delta, delta);
    float radius = circle.r;

    if (squaredDistance > radius * radius)
        // No collision
        return 0;

    float d = sqrt(squaredDistance);

    if (d > 0.0f) {
        // Collision
        m->normal = scalarMultiply(delta, 1.0 / d);
        m->penetration = radius - d;
        return 1;
    }

    // Collision, but the center is inside the AABB, we need to chose the closest side to estimate
    // the normal vector
    float left = circleCenter.x - aabb_xmin;
    float right = aabb_xmax - circleCenter.x;
    float bottom = circleCenter.y - aabb_xmin;
    float top = aabb_ymax - circleCenter.y;

    float minDist = left;
    m->normal = (Vector2D){-1.0f, 0.0f};

    if (right < minDist) {
        minDist = right;
        m->normal = (Vector2D){1.0f, 0.0f};
    }
    if (bottom < minDist) {
        minDist = bottom;
        m->normal = (Vector2D){0.0f, -1.0f};
    }
    if (top < minDist) {
        minDist = top;
        m->normal = (Vector2D){0.0f, 1.0f};
    }

    m->penetration = radius + minDist;
    return 1;
}
