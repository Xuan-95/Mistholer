#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int   WINDOW_WIDTH;
    int   WINDOW_HEIGHT;
    float FPS;
} Config;

void parseConfig(const char *filename, Config *cfg);

#endif
