//
// Created by John Rockwell on 2/6/25.
//

#ifndef CLIENT_H
#define CLIENT_H

#include "camera/camera_controller.h"
#include "level/level.h"

class GameClient {
public:
    GameClient();
    ~GameClient();

    void processLoop() const;

private:
    GameLevel *currentLevel;
    CameraController *cameraController;

    void processInputs() const;
    void processRender() const;
    void printDebugToScreen() const;
};

#endif //CLIENT_H
