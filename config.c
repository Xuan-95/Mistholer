#include "config.h"
#include "common.h"
#include <string.h>

#define MAX_LINE 256

void parseConfig(const char *filename, Config *cfg) {
    FILE *f = fopen(filename, "r");

    if (!f)
        return;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        // Ignore comments, sections and empty lines
        if (line[0] == '#' || line[0] == '[' || line[0] == '\n') {
            continue;
        }

        char key[64], value[128];
        if (sscanf(line, "%63[^=]=%127s", key, value)) {
            if (strcmp(key, "width") == 0) {
                cfg->WINDOW_WIDTH = atoi(value);
            }
            if (strcmp(key, "height") == 0) {
                cfg->WINDOW_HEIGHT = atoi(value);
            }
            if (strcmp(key, "fps") == 0) {
                cfg->FPS = atof(value);
            }
        }
    }
    fclose(f);
}
