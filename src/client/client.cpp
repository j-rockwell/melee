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

    currentLevel = new GameLevel("dev_level");
    currentLevel->generateStarterScene();

    player = new Player("steven", true);

    DisableCursor();
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
}

GameClient::~GameClient() {
    delete currentLevel;

    CloseWindow();
}

void GameClient::processLoop() const {
    while (!WindowShouldClose()) {
        processRender();
        tickEntities();
    }
}

void GameClient::printDebugToScreen() const {
    DrawText(TextFormat("Current Level: %s", currentLevel->getLevelName()), 10, 10, 12, GRAY);
    DrawText(TextFormat("Allocated Chunks: %d", currentLevel->getChunkCount()), 10, 25, 12, GRAY);
    DrawText(TextFormat("Allocated Nodes: %d", currentLevel->getNodeCount()), 10, 40, 12, GRAY);
    DrawText(TextFormat("FPS: %i", GetFPS()), 10, 55, 12, GRAY);
    DrawText(TextFormat("Current Coordinates - X: %i, Y: %i, Z: %i",
        (int)player->getPosition().x,
        (int)player->getPosition().y,
        (int)player->getPosition().z), 10, 70, 12, GRAY);
}

void GameClient::tickEntities() const {
    if (player) {
        player->tick(GetFrameTime());
    }
}

void GameClient::processRender() const {
    BeginDrawing();
    ClearBackground(BLACK);

    // TODO: remove or make this toggleable
    printDebugToScreen();

    if (player == nullptr) {
        return;
    }

    BeginMode3D(player->getCamera());

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
