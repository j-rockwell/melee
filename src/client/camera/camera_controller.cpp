//
// Created by John Rockwell on 2/6/25.
//

#include "camera_controller.h"
#include "../util/game_math.h"

CameraController::CameraController(Camera3D &cam) : yaw(0.0f), pitch(0.0f), sensitivity(1.0f), flySpeed(6.0f), camera(cam) {}

void CameraController::updateMouse(const Vector2 delta) {
    yaw += delta.x * sensitivity;
    pitch -= delta.y * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    Vector3 forwardVector;
    forwardVector.x = cosf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forwardVector.y = sinf(DEG2RAD * pitch);
    forwardVector.z = sinf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forwardVector = MathHelper::NormalizeVector3D(forwardVector);

    camera.target = MathHelper::AddVector3D(camera.position, forwardVector);
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);
}

void CameraController::updateCameraY(const bool up) {
    if (up) {
        camera.position.y += flySpeed * GetFrameTime();
        return;
    }

    camera.position.y -= flySpeed * GetFrameTime();
}
