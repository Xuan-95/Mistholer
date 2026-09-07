#include "loader.h"
#include <string.h>

void initBodyDesc(BodyDesc *desc) {
    *desc                      = (BodyDesc){0};
    desc->material.density     = 1.0;
    desc->material.restitution = 1.0;
}

char *readTextFile(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file)
        return NULL;

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long length = ftell(file);
    if (length < 0) {
        fclose(file);
        return NULL;
    }

    rewind(file);

    char  *text = ALLOCATE(char, (size_t)length + 1);

    size_t bytesRead = fread(text, 1, (size_t)length, file);
    if (ferror(file)) {
        free(text);
        fclose(file);
        return NULL;
    }

    text[bytesRead] = '\0';
    fclose(file);
    return text;
}

bool readVector(const cJSON *json, Vector2D *out) {
    if (!cJSON_IsArray(json) || cJSON_GetArraySize(json) != 2) {
        return false;
    }
    const cJSON *x = cJSON_GetArrayItem(json, 0);
    const cJSON *y = cJSON_GetArrayItem(json, 1);

    if (!cJSON_IsNumber(x) || !cJSON_IsNumber(y))
        return false;

    if (!isfinite(x->valuedouble) || !isfinite(y->valuedouble))
        return false;

    *out = (Vector2D){x->valuedouble, y->valuedouble};
    return true;
}

bool readShape(const cJSON *json, Shape *out) {
    if (!cJSON_IsObject(json))
        return false;

    const cJSON *type = cJSON_GetObjectItem(json, "type");
    if (!cJSON_IsString(type))
        return false;

    Shape shape = {0};

    if (strcmp(type->valuestring, "circle") == 0) {
        const cJSON *radius_json = cJSON_GetObjectItemCaseSensitive(json, "radius");
        double       radius      = cJSON_GetNumberValue(radius_json);

        if (!isfinite(radius) || radius < 0.0)
            return false;

        initShape(&shape, SHAPE_CIRCLE, radius);

    } else if (strcmp(type->valuestring, "aabb") == 0) {
        const cJSON *halfheight_json = cJSON_GetObjectItemCaseSensitive(json, "halfHeight");
        const cJSON *halfwidth_json  = cJSON_GetObjectItemCaseSensitive(json, "halfWidth");
        double       halfheight      = cJSON_GetNumberValue(halfheight_json);
        double       halfwidth       = cJSON_GetNumberValue(halfwidth_json);

        if (!isfinite(halfheight) || halfheight <= 0.0)
            return false;
        if (!isfinite(halfwidth) || halfwidth <= 0.0)
            return false;

        initShape(&shape, SHAPE_AABB, halfwidth, halfheight);
    } else {
        fprintf(stderr, "Shape %s not supported", type->valuestring);
        return false;
    }

    *out = shape;
    return true;
}

bool readBodyDesc(const cJSON *json, BodyDesc *out) {
    if (!cJSON_IsObject(json))
        return false;

    BodyDesc desc;
    initBodyDesc(&desc);

    const cJSON *position = cJSON_GetObjectItemCaseSensitive(json, "position");
    const cJSON *velocity = cJSON_GetObjectItemCaseSensitive(json, "velocity");
    const cJSON *shape    = cJSON_GetObjectItemCaseSensitive(json, "shape");

    if (!readVector(position, &desc.position))
        return false;

    if (velocity && !readVector(velocity, &desc.velocity))
        return false;

    if (!cJSON_IsObject(shape))
        return false;

    if (!readShape(shape, &desc.shape))
        return false;

    *out = desc;
    return true;
}

bool loadSceneFromJson(const char *path, Scene *outScene) {
    char *json_content = readTextFile(path);
    if (!json_content) {
        fprintf(stderr, "Unable to load scene from %s", path);
        return false;
    }

    cJSON *root = cJSON_Parse(json_content);

    free(json_content);
    if (!root) {
        fprintf(stderr, "Invalid JSON in %s\n", path);
        return false;
    }

    Scene temp;
    initScene(&temp);

    bool success = false;
    if (!cJSON_IsObject(root))
        goto cleanup;

    const cJSON *bodies = cJSON_GetObjectItemCaseSensitive(root, "bodies");
    if (!cJSON_IsArray(bodies))
        goto cleanup;

    const cJSON *item;
    int          idx = 0;

    cJSON_ArrayForEach(item, bodies) {
        BodyDesc desc;

        if (!readBodyDesc(item, &desc)) {
            fprintf(stderr, "Body %d: description not valid", idx);
            goto cleanup;
        }

        if (!sceneCreateBody(&temp, &desc)) {
            goto cleanup;
        }
        idx++;
    }
    destroyScene(outScene);
    *outScene = temp;
    success   = true;

cleanup:
    cJSON_Delete(root);
    if (!success) {
        destroyScene(&temp);
    }
    return success;
}
