#include "physics/scene.h"
#include "physics/body.h"
#include "core/memory.h"

void initScene(Scene *scene) {
    scene->bodies   = NULL;
    scene->count    = 0;
    scene->capacity = 0;
    scene->gravity  = 980;
}

void destroyScene(Scene *scene) {
    FREE_ARRAY(Body, scene->bodies, scene->capacity);
    scene->bodies = NULL;
}

void addBody(Scene *scene, Body *body) {
    if (scene->capacity < scene->count + 1) {
        int oldCapacity = scene->capacity;
        scene->capacity = GROW_CAPACITY(oldCapacity);
        scene->bodies   = GROW_ARRAY(Body, scene->bodies, oldCapacity, scene->capacity);
    }
    scene->bodies[scene->count] = *body;
    scene->count++;
}

bool sceneCreateBody(Scene *scene, BodyDesc *desc) {
    Body body = {0};
    initBody(&body, desc);
    addBody(scene, &body);
    return true;
}
