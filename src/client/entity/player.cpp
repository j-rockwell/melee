//
// Created by John Rockwell on 2/9/25.
//

#include "player.h"

Player::Player(std::string  name, const bool local)
    : name(std::move(name)),
      health(100.0f),
      local(local)
{
    // TODO: have an actual start pos
    movementController = new PlayerMovementController({0.f, 150.f, 0.0f});
}

Player::~Player() {
    delete movementController;
}

void Player::tick(const float deltaTime) const {
    if (movementController) {
        movementController->update(deltaTime);
    }
}
