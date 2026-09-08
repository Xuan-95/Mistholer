#include "input.h"
#include <raylib.h>

void handleInput(Scene *scene, UIState *ui) {
    // Create a body with click
    Rectangle toolbar   = (Rectangle){1700, 20, 160, 64};
    Vector2   mouse_pos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mouse_pos, toolbar)) {
        Vector2D pos   = toVector2D(mouse_pos);
        Body    *body  = malloc(sizeof(Body));
        Shape   *shape = malloc(sizeof(Shape));
        switch (ui->selectedShape) {
        case SHAPE_CIRCLE: {
            ShapeType type   = SHAPE_CIRCLE;
            double    radius = rand() % 50 + 1;
            initShape(shape, type, radius);
            BodyDesc desc;
            initBodyDesc(&desc);
            desc.shape    = *shape;
            desc.position = pos;
            initBody(body, &desc);
            addBody(scene, body);
            break;
        }
        case SHAPE_AABB: {
            ShapeType type       = SHAPE_AABB;
            double    halfheight = rand() % 50 + 1;
            double    halfwidth  = rand() % 50 + 1;
            initShape(shape, type, halfwidth, halfheight);
            BodyDesc desc;
            initBodyDesc(&desc);
            desc.shape    = *shape;
            desc.position = pos;
            initBody(body, &desc);
            addBody(scene, body);
            break;
        }
        default:
            fprintf(stderr, "Shape not supported\n");
            break;
        }
    }
}
