//
// Created by John Rockwell on 2/6/25.
//

#ifndef LEVEL_H
#define LEVEL_H
#include "chunk.h"
#include "node.h"
#include "../render/greedy_mesh.h"

constexpr int LEVEL_WIDTH = 512;
constexpr int LEVEL_HEIGHT = 128;
constexpr int CHUNK_COUNT_X = LEVEL_WIDTH / CHUNK_WIDTH;
constexpr int CHUNK_COUNT_Z = LEVEL_WIDTH / CHUNK_WIDTH;

class GameLevel {
public:
    explicit GameLevel(const char* levelName);
    ~GameLevel();

    void generateStarterScene();
    void generateChunkMeshes();

    Node getNode(int x, int y, int z) const;
    void setNode(int x, int y, int z, NodeType type) const;
    // TODO: Implement a way to set nodes with custom attributes

    const Chunk* getChunkAt(int x, int z) const;

    uint32_t getChunkCount() const { return chunks.size(); }
    uint64_t getNodeCount() const { return CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH * getChunkCount(); }

    const char* getLevelName() const { return levelName; }

    const Mesh& getChunkMeshAt(int x, int z) const;

private:
    const char* levelName;
    std::vector<Chunk*> chunks;
    std::vector<Mesh> meshes;

    static int getChunkIndex(const int x, const int z) {
        return x + CHUNK_COUNT_X * z;
    }
};

#endif //LEVEL_H
