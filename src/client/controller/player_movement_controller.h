//
// Created by John Rockwell on 2/10/25.
//

#ifndef PLAYER_MOVEMENT_CONTROLLER_H
#define PLAYER_MOVEMENT_CONTROLLER_H
#include <raylib.h>

class PlayerMovementController {
public:
    explicit PlayerMovementController(const Vector3& startPos);
    ~PlayerMovementController();

    Camera3D getCamera() const { return camera;}

    Vector3 getPosition() const { return position; }
    Vector3 getVelocity() const { return velocity; }

    void update(float deltaTime);
    void setPosition(const Vector3& pos);
    void setCameraPosition(const Vector3& pos);

protected:
    void updateCamera();

private:
    Camera3D camera;

    Vector3 position;
    Vector3 velocity;
    Vector3 cameraOffset;

    float yaw;
    float pitch;
    float lookSensitivity;

    float moveAccel;
    float maxSpeed;
    float friction;
    float gravity;
    float jumpImpulse;
    bool onGround;
};

#endif //PLAYER_MOVEMENT_CONTROLLER_H
