//
// Created by John Rockwell on 2/6/25.
//

#include "level.h"

GameLevel::GameLevel(const char* levelName) : levelName(levelName) {
    chunks.resize(CHUNK_COUNT_X * CHUNK_COUNT_Z, nullptr);

    int allocated = 0;
    for (int cz = 0; cz < CHUNK_COUNT_Z; cz++) {
        for (int cx = 0; cx < CHUNK_COUNT_X; cx++) {
            const int index = getChunkIndex(cx, cz);
            chunks[index] = new Chunk(cx, cz);
            allocated += 1;
        }
    }
}

GameLevel::~GameLevel() {
    // dealloc chunk memory
    for (const Chunk* chunk : chunks) {
        delete chunk;
    }

    // dealloc mesh from raylib
    for (const Mesh &mesh : meshes) {
        UnloadMesh(mesh);
    }
}

void GameLevel::generateStarterScene() {
    for (Chunk* chunk : chunks) {
        if (!chunk) continue;
        chunk->generateStarterTerrain();
    }

    generateChunkMeshes();
}

void GameLevel::generateChunkMeshes() {
    meshes.resize(CHUNK_COUNT_X * CHUNK_COUNT_Z);

    for (size_t i = 0; i < chunks.size(); i++) {
        if (chunks[i]) {
            meshes[i] = GreedyMesher::generateChunkMesh(this, chunks[i]);
        }
    }
}

Node GameLevel::getNode(const int x, const int y, const int z) const {
    if (x < 0 || x >= LEVEL_WIDTH || z < 0 || z >= LEVEL_WIDTH) {
        return Node{AIR};
    }

    if (y < 0 || y >= LEVEL_HEIGHT) {
        return Node{AIR};
    }

    const int chunkX = x / CHUNK_WIDTH;
    const int chunkZ = z / CHUNK_WIDTH;
    const int relX = x % CHUNK_WIDTH;
    const int relZ = z % CHUNK_WIDTH;
    const int index = getChunkIndex(chunkX, chunkZ);
    return chunks[index]->getNode(relX, y, relZ);
}

void GameLevel::setNode(const int x, const int y, const int z, const NodeType type) const {
    if (x < 0 || x >= LEVEL_WIDTH || z < 0 || z >= LEVEL_WIDTH) {
        // TODO: throw error
        return;
    }

    if (y < 0 || y >= LEVEL_HEIGHT) {
        // TODO: throw error
        return;
    }

    const int chunkX = x / CHUNK_WIDTH;
    const int chunkZ = z / CHUNK_WIDTH;
    const int relX = x % CHUNK_WIDTH;
    const int relZ = z % CHUNK_WIDTH;
    const int index = getChunkIndex(chunkX, chunkZ);
    chunks[index]->setNode(relX, y, relZ, type);
}

const Chunk* GameLevel::getChunkAt(const int x, const int z) const {
    if (x < 0 || x >= CHUNK_COUNT_X || z < 0 || z >= CHUNK_COUNT_Z) {
        return nullptr;
    }

    return chunks[getChunkIndex(x, z)];
}

const Mesh& GameLevel::getChunkMeshAt(const int x, const int z) const {
    const int index = getChunkIndex(x, z);
    return meshes[index];
}