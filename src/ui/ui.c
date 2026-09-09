#include "ui/ui.h"
#include "physics/body.h"
#include "raylib.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#pragma GCC diagnostic pop

void drawUI(UIState *ui) {
    if (GuiButton((Rectangle){1700, 20, 160, 32}, "Circle")) {
        ui->selectedShape = SHAPE_CIRCLE;
        TraceLog(LOG_INFO, "Circle selected");
    } else if (GuiButton((Rectangle){1700, 53, 160, 32}, "AABB")) {
        ui->selectedShape = SHAPE_AABB;
        TraceLog(LOG_INFO, "AABB selected");
    }
}
