#ifndef LOADER_H
#define LOADER_H

#include "cJSON.h"
#include "core/common.h"
#include "core/memory.h"
#include "physics/scene.h"

void  initBodyDesc(BodyDesc *desc);
char *readTextFile(const char *path);
bool  readVector(const cJSON *json, Vector2D *out);
bool  readBodyDesc(const cJSON *json, BodyDesc *desc);
bool  loadSceneFromJson(const char *path, Scene *outScene);

#endif // !LOADER_H
