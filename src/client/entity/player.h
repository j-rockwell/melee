//
// Created by John Rockwell on 2/9/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include <string>

#include "../controller/player_movement_controller.h"

/*
 * Possible fields?
 * ID
 * Name
 * Position (Vec3D)
 * Health
 * Attributes
 */

class Player {
public:
    Player(std::string  name, bool local);
    ~Player();

    std::string getName() { return name; }
    Vector3 getPosition() const { return movementController->getPosition(); }
    Vector3 getVelocity() const { return movementController->getVelocity(); }
    Camera3D getCamera() const { return movementController->getCamera(); }
    PlayerMovementController getMovementController() const { return *movementController; };
    bool isLocal() const { return local; }
    bool isDead() const { return health <= 0.0f; }

    void setPosition(const Vector3& pos) { movementController->setPosition(pos); }
    void tick(float deltaTime) const;

protected:
    std::string name;
    float health;
    bool local;
    PlayerMovementController* movementController;
};

#endif //PLAYER_H
