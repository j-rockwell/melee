//
// Created by John Rockwell on 2/9/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include <string>

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
    Player(const std::string& name, bool local);
    ~Player();

    std::string getName() { return name; }
    Vector3 getPosition() const { return position; }
    bool isLocal() const { return local; }
    bool isDead() const { return health <= 0.0f; }

    void setPosition(const Vector3& pos);
    void setRotation(const Vector3& rot);
    void tick(float deltaTime);

protected:
    std::string name;
    float health;
    Vector3 position;
    Vector3 rotation;
    Vector3 velocity;
    bool local;
};



#endif //PLAYER_H
