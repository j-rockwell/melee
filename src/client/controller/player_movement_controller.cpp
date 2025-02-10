//
// Created by John Rockwell on 2/10/25.
//

#include "player_movement_controller.h"
#include "../util/game_math.h"

#ifndef DEG2RAD
    #define DEG2RAD (PI / 180.0f)
#endif

/*
 * Some movement configs:
 * minecraft:
 *  moveAccel 50.0
 *  maxSpeed 5.0
 *  friction 15.0
 *  gravity 20.0
 *  jumpImpulse 5.0
 *
 * quake:
 *  150.0
 *  12.0
 *  5.0
 *  20.0
 *  8.0
 */

PlayerMovementController::PlayerMovementController(const Vector3& startPos) :
position(startPos),
velocity({0.f, 0.f, 0.f}),
yaw(0.f),
pitch(0.f),
moveAccel(150.0f),
maxSpeed(12.0f),
friction(5.0f),
gravity(20.0f),
jumpImpulse(8.0f),
lookSensitivity(1.0f),
onGround(true)
{
    cameraOffset = {0.0f, 1.5f, 0.0f}; // player eye height

    Camera3D cam;
    cam.position = MathHelper::AddVector3D(startPos, cameraOffset);
    cam.target = MathHelper::AddVector3D(startPos, {0.0f, 0.0f, 1.0f});
    cam.up = {0.0f, 1.0f, 0.0f};
    cam.fovy = 90.0f;
    cam.projection = CAMERA_PERSPECTIVE;
    camera = cam;
}

PlayerMovementController::~PlayerMovementController() {

}

void PlayerMovementController::update(const float deltaTime) {
    // handle mouse input
    const Vector2 mouseDelta = GetMouseDelta();
    yaw += mouseDelta.x * lookSensitivity;
    pitch -= mouseDelta.y * lookSensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // compute forward vec
    Vector3 forward;
    forward.x = cosf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forward.y = sinf(DEG2RAD * pitch);
    forward.z = sinf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forward = MathHelper::NormalizeVector3D(forward);

    // compute right vec
    const Vector3 right = MathHelper::NormalizeVector3D(MathHelper::CrossVector3D(forward, {0.0f, 1.0f, 0.0f}));

    float forwardInput = 0.0f;
    if (IsKeyDown(KEY_W)) forwardInput += 1.0f;
    if (IsKeyDown(KEY_S)) forwardInput -= 1.0f;

    float strafeInput = 0.0f;
    if (IsKeyDown(KEY_D)) strafeInput += 1.0f;
    if (IsKeyDown(KEY_A)) strafeInput -= 1.0f;

    // compute horizontal movement velocity
    if (onGround) {
        // combine strafe movements
        Vector3 wishDir = { forward.x * forwardInput + right.x * strafeInput,
                             0.0f,
                             forward.z * forwardInput + right.z * strafeInput };

        if (MathHelper::LengthVector3D(wishDir) > 0) {
            wishDir = MathHelper::NormalizeVector3D(wishDir);
        }

        velocity.x += wishDir.x * moveAccel * deltaTime;
        velocity.z += wishDir.z * moveAccel * deltaTime;
    } else {
        const float forwardAccel = moveAccel;
        const float strafeAccel  = moveAccel * 0.3f;

        // apply forward acceleration
        velocity.x += forward.x * forwardInput * forwardAccel * deltaTime;
        velocity.z += forward.z * forwardInput * forwardAccel * deltaTime;

        // apply left/right acceleration
        velocity.x += right.x * strafeInput * strafeAccel * deltaTime;
        velocity.z += right.z * strafeInput * strafeAccel * deltaTime;
    }

    // speed clamping
    const Vector3 horizVel = { velocity.x, 0.0f, velocity.z };
    const float horizSpeed = MathHelper::LengthVector3D(horizVel);
    if (horizSpeed > maxSpeed) {
        const float scaleFactor = maxSpeed / horizSpeed;
        velocity.x *= scaleFactor;
        velocity.z *= scaleFactor;
    }

    // apply jump velocity
    if (IsKeyPressed(KEY_SPACE) && onGround) {
        velocity.y = jumpImpulse;
        onGround = false;
    }

    // apply gravity
    velocity.y -= gravity * deltaTime;

    // apply ground friction
    if (onGround) {
        const float frictionAmount = friction * deltaTime;

        if (horizSpeed > frictionAmount) {
            const float scaleFactor = (horizSpeed - frictionAmount) / horizSpeed;
            velocity.x *= scaleFactor;
            velocity.z *= scaleFactor;
        } else {
            velocity.x = 0;
            velocity.z = 0;
        }
    }

    // apply to position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    position.z += velocity.z * deltaTime;

    // TODO: remove this hacky ground collision check (query world for node to determine floor height (cast from foot pos?))
    constexpr float groundHeight = 130.0f;
    if (position.y <= groundHeight) {
        position.y = groundHeight;
        velocity.y = 0;
        onGround = true;
    }

    camera.position = MathHelper::AddVector3D(position, cameraOffset);
    updateCamera();
}

void PlayerMovementController::updateCamera() {
    Vector3 forward;
    forward.x = cosf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forward.y = sinf(DEG2RAD * pitch);
    forward.z = sinf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forward = MathHelper::NormalizeVector3D(forward);
    camera.target = MathHelper::AddVector3D(camera.position, forward);
}

void PlayerMovementController::setPosition(const Vector3 &pos) {
    position = pos;
    camera.position = MathHelper::AddVector3D(pos, cameraOffset);
    updateCamera();
}

void PlayerMovementController::setCameraPosition(const Vector3 &pos) {
    camera.position = pos;
    updateCamera();
}