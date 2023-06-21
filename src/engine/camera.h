#ifndef CAMERA_H
#define CAMERA_H

Camera2D createCamera2D(Vector2 target, Vector2 offset);
void updateCamera2D(Camera2D* pCam, Vector2 target);

#endif