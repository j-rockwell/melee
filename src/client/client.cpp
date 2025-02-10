//
// Created by John Rockwell on 2/6/25.
//

#include <raylib.h>
#include "client.h"
#include "entity/player.h"
#include "level/level.h"
#include "render/greedy_mesh.h"
#include "render/wireframe.h"
#include "util/game_math.h"

// TODO: Totally re-implement this. We're using this as a tester class currently
// In a real game project we will not want to initialize the Game Client and Raylib
// renderer here as it would be established when the main menu booted up and passed over
// to this Game Client to handle game render logic.
//
// Alas, here we are for now.

GameClient::GameClient() {
    InitWindow(1280, 720, "Melee Game Demo");
    SetTargetFPS(1000);                                             // Need to cap this off because infinite FPS target
                                                                    // will make your shit EXPLODE!!!
    Camera3D camera;
    camera.position = { 0.0f, 132.0f, 0.0f };
    camera.target   = { 512.0f, 150.0f, 512.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    cameraController = new CameraController(camera);

    currentLevel = new GameLevel("dev_level");
    currentLevel->generateStarterScene();

    player = new Player("steven", true);

    DisableCursor();
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
}

GameClient::~GameClient() {
    delete cameraController;
    delete currentLevel;

    CloseWindow();
}

void GameClient::processLoop() const {
    while (!WindowShouldClose()) {
        processInputs();
        processRender();
    }
}

void GameClient::processInputs() const {
    // mouse movement for flycam
    const Vector2 mouseDelta = GetMouseDelta();
    cameraController->updateMouse(mouseDelta);

    // allow up/down travel (raylib fps camera supports WASD out of the box)
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        cameraController->updateCameraY(false);
    }

    if (IsKeyDown(KEY_SPACE)) {
        cameraController->updateCameraY(true);
    }

    // TODO: remove hack to test player movement rendering
    player->setPosition(cameraController->getCamera().position);
    player->setRotation(cameraController->getCamera().target);
}

void GameClient::printDebugToScreen() const {
    DrawText(TextFormat("Current Level: %s", currentLevel->getLevelName()), 10, 10, 12, GRAY);
    DrawText(TextFormat("Allocated Chunks: %d", currentLevel->getChunkCount()), 10, 25, 12, GRAY);
    DrawText(TextFormat("Allocated Nodes: %d", currentLevel->getNodeCount()), 10, 40, 12, GRAY);
    DrawText(TextFormat("FPS: %i", GetFPS()), 10, 55, 12, GRAY);
    DrawText(TextFormat("Current Coordinates - X: %i, Y: %i, Z: %i",
        (int)cameraController->getCamera().position.x,
        (int)cameraController->getCamera().position.y,
        (int)cameraController->getCamera().position.z), 10, 70, 12, GRAY);
}

void GameClient::processRender() const {
    BeginDrawing();
    ClearBackground(BLACK);

    // TODO: remove or make this toggleable
    printDebugToScreen();

    BeginMode3D(cameraController->getCamera());

    // render a capsule to signify the player
    if (player) {
        const Vector3 capStart = Vector3({player->getPosition().x+5, player->getPosition().y-2.0f, player->getPosition().z});
        const Vector3 capEnd = Vector3({player->getPosition().x+5, player->getPosition().y, player->getPosition().z});
        DrawCapsule(capStart, capEnd, 1.0f, 64.0f, 3.0f, RED);
    }

    // test chunk rendering with meshes
    for (int cz = 0; cz < CHUNK_COUNT_Z; cz++) {
        for (int cx = 0; cx < CHUNK_COUNT_X; cx++) {
            const Chunk* chunk = currentLevel->getChunkAt(cx, cz);
            if (!chunk) continue;

            if (const Mesh& chunkMesh = currentLevel->getChunkMeshAt(cx, cz); chunkMesh.vertexCount > 0) {
                const Material defaultMat = LoadMaterialDefault();
                const float tx = chunk->getChunkX() * CHUNK_WIDTH;
                const float tz = chunk->getChunkZ() * CHUNK_WIDTH;
                const Matrix translation = MathHelper::MatrixTranslate(tx, 64.0f, tz);

                DrawMesh(chunkMesh, defaultMat, translation);
                // TODO: make this toggleable
                WireframeHelper::DrawMeshWireframe(chunkMesh, translation, GREEN);
                UnloadMaterial(defaultMat);
            }
        }
    }

    EndMode3D();
    EndDrawing();
}
