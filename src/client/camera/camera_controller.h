//
// Created by John Rockwell on 2/6/25.
//

#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include <raylib.h>

class CameraController {
public:
    explicit CameraController(Camera3D &cam);

    float yaw;
    float pitch;
    float sensitivity;
    float flySpeed;

    Camera3D getCamera() const { return camera; }

    void updateMouse(Vector2 delta);
    void updateCameraY(bool up);
private:
    Camera3D camera;
};

#endif //CAMERA_CONTROLLER_H
