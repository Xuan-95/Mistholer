#ifndef UI_H
#define UI_H

#include "physics/scene.h"

typedef struct {
    ShapeType selectedShape;
} UIState;

void drawUI(UIState *ui);

#endif // UI_H
