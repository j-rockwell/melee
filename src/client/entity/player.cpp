//
// Created by John Rockwell on 2/9/25.
//

#include "player.h"

Player::Player(const std::string& name, const bool local)
    : name(name),
      health(100.0f),
      position({0.0f, 0.0f, 0.0f}),
      rotation({0.0f, 0.0f, 0.0f}),
      velocity({0.0f, 0.0f, 0.0f}),
      local(local)
{
    if (local) {
        position = {0.0f, 70.0f, 0.0f};
    }
}

void Player::tick(float deltaTime) {

}

void Player::setPosition(const Vector3& pos) {
    position = pos;
}

void Player::setRotation(const Vector3& rot) {
    rotation = rot;
}