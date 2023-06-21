#include <raylib.h>
#include "./camera.h"

Camera2D createCamera2D(Vector2 target, Vector2 offset)
{
    // Create a 2D camera
    Camera2D cam = { 0 };
    cam.target = target;
    cam.offset = offset;
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;

    return cam;
}

void updateCamera2D(Camera2D* pCam, Vector2 target)
{
    pCam->target = (Vector2){ target.x, target.y };
}
