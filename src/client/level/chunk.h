//
// Created by John Rockwell on 2/6/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "node.h"

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 128;
constexpr int CHUNK_DEFAULT_GROUND_LEVEL = 64;

class Chunk {
public:
    Chunk(int x, int z);
    ~Chunk();

    Node getNode(int x, int y, int z) const;
    void setNode(int x, int y, int z, NodeType type);
    // TODO: Implement a setNode function that allows for complex types

    void generateStarterTerrain();

    std::vector<Node> getNodes() const { return nodes; }

    int getChunkX() const { return x; }
    int getChunkZ() const { return z; }

private:
    int x;
    int z;
    std::vector<Node> nodes;

    static int index(const int x, const int y, const int z) {
        return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z);
    }
};

#endif //CHUNK_H
