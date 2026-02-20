#include "scene.h"
#include "memory.h"

void initScene(Scene *scene) {
    scene->bodies = NULL;
    scene->count = 0;
    scene->capacity = 0;
}

void addBody(Scene *scene, Body *body) {
    if (scene->capacity < scene->count + 1) {
        int oldCapacity = scene->capacity;
        scene->capacity = GROW_CAPACITY(oldCapacity);
        scene->bodies = GROW_ARRAY(Body, scene->bodies, oldCapacity, scene->capacity);
    }
    scene->bodies[scene->count] = *body;
    scene->count++;
}
