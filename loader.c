#include "loader.h"
#include "cJSON.h"
#include <string.h>

void initBodyDesc(BodyDesc *desc) {
    *desc                      = (BodyDesc){0};
    desc->gravityScale         = 1.0;
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

// Parse array from cJSON field
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

// Parse positive (strictly greater than zero) number from cJSON field.
bool readPositiveNonZeroNumber(const cJSON *json, const char *field, double *out) {
    const cJSON *item = cJSON_GetObjectItemCaseSensitive(json, field);
    double       num  = cJSON_GetNumberValue(item);

    if (!isfinite(num) || num < 0.0)
        return false;

    *out = num;
    return true;
}

/* Parse the Shape from the JSON file that will be included in the BodyDesc object.
 * The Shape will be different based on the 'type' field of the JSON */
bool readShape(const cJSON *json, Shape *out) {
    if (!cJSON_IsObject(json))
        return false;

    const cJSON *type = cJSON_GetObjectItem(json, "type");
    if (!cJSON_IsString(type))
        return false;

    Shape shape = {0};

    if (strcmp(type->valuestring, "circle") == 0) {
        double radius;
        if (!readPositiveNonZeroNumber(json, "radius", &radius))
            return false;
        initShape(&shape, SHAPE_CIRCLE, radius);

    } else if (strcmp(type->valuestring, "aabb") == 0) {
        double halfheight, halfwidth;
        if (!readPositiveNonZeroNumber(json, "halfWidth", &halfwidth) ||
            !readPositiveNonZeroNumber(json, "halfHeight", &halfheight))
            return false;
        initShape(&shape, SHAPE_AABB, halfwidth, halfheight);

    } else {
        fprintf(stderr, "Shape %s not supported", type->valuestring);
        return false;
    }

    *out = shape;
    return true;
}

/* Parse one element of 'bodies' object written into the JSON file */
bool readBodyDesc(const cJSON *json, BodyDesc *out) {
    if (!cJSON_IsObject(json))
        return false;

    BodyDesc desc;
    initBodyDesc(&desc);

    const cJSON *position           = cJSON_GetObjectItemCaseSensitive(json, "position");
    const cJSON *velocity           = cJSON_GetObjectItemCaseSensitive(json, "velocity");
    const cJSON *shape              = cJSON_GetObjectItemCaseSensitive(json, "shape");
    const cJSON *gravity_scale_json = cJSON_GetObjectItemCaseSensitive(json, "gravity_scale");
    const cJSON *isStatic           = cJSON_GetObjectItemCaseSensitive(json, "static");

    if (!readVector(position, &desc.position))
        return false;

    if (velocity && !readVector(velocity, &desc.velocity))
        return false;

    if (gravity_scale_json) {
        double gravity_scale = cJSON_GetNumberValue(gravity_scale_json);
        if (!isfinite(gravity_scale))
            return false;
        desc.gravityScale = gravity_scale;
    }

    if (isStatic) {
        if (!cJSON_IsBool(isStatic))
            return false;
        desc.is_static = cJSON_IsTrue(isStatic);
    }

    if (!cJSON_IsObject(shape))
        return false;

    if (!readShape(shape, &desc.shape))
        return false;

    *out = desc;
    return true;
}

/* Load from a JSON file the description of the scene.
 * The function load the file, build a list of BodyDesc objects and load the Body objects inside the Scene. */
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

    // Parse gravity
    const cJSON *scene_gravity_json = cJSON_GetObjectItemCaseSensitive(root, "gravity");
    if (!scene_gravity_json)
        goto cleanup;

    double gravity = cJSON_GetNumberValue(scene_gravity_json);
    if (!isfinite(gravity)) {
        fprintf(stderr, "Gravity field not valid\n");
        goto cleanup;
    }

    temp.gravity = gravity;

    // Parse list of bodies
    const cJSON *bodies = cJSON_GetObjectItemCaseSensitive(root, "bodies");
    if (!cJSON_IsArray(bodies))
        goto cleanup;

    const cJSON *item;
    int          idx = 0;

    // Iterate over each body from the JSON file. Build the BodyDesc and add it to the scene
    cJSON_ArrayForEach(item, bodies) {
        BodyDesc desc;

        if (!readBodyDesc(item, &desc)) {
            fprintf(stderr, "Body %d: description not valid\n", idx);
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
