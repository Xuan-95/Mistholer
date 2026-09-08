#ifndef SCENE_H
#define SCENE_H

#include "body.h"
#include "common.h"

typedef struct {
    Body  *bodies;
    int    count;    // Number of active bodies
    int    capacity; // Body capacity
    double gravity;
} Scene;

void initScene(Scene *scene);
void destroyScene(Scene *scene);
void addBody(Scene *scene, Body *body);
bool sceneCreateBody(Scene *scene, BodyDesc *desc);

#endif // !SCENE_H
