#ifndef SCENE_H
#define SCENE_H

#include "body.h"

typedef struct {
    Body *bodies;
    int count;    // Number of active bodies
    int capacity; // Body capacity
} Scene;

void initScene(Scene *scene);
void addBody(Scene *scene, Body *body);

#endif // !SCENE_H
