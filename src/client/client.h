//
// Created by John Rockwell on 2/6/25.
//

#ifndef CLIENT_H
#define CLIENT_H

#include "entity/player.h"
#include "level/level.h"

class GameClient {
public:
    GameClient();
    ~GameClient();

    Player getPlayer() const { return *player; }

    void processLoop() const;

private:
    GameLevel *currentLevel;
    Player *player;

    void tickEntities() const;
    void processRender() const;
    void printDebugToScreen() const;
};

#endif //CLIENT_H
