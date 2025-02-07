//
// Created by John Rockwell on 2/6/25.
//

#include "chunk.h"

Chunk::Chunk(const int x, const int z) : x(x), z(z) {
    constexpr int size = CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT;
    nodes.resize(size, Node{AIR});
}

Chunk::~Chunk() = default;

Node Chunk::getNode(const int x, const int y, const int z) const {
    if (x < 0 || x >= CHUNK_WIDTH || z < 0 || z >= CHUNK_WIDTH) {
        return Node{AIR};
    }

    if (y < 0 || y >= CHUNK_HEIGHT) {
        return Node{AIR};
    }

    return nodes[index(x, y, z)];
}

void Chunk::setNode(const int x, const int y, const int z, const NodeType type) {
    if (x < 0 || x >= CHUNK_WIDTH || z < 0 || z >= CHUNK_WIDTH) {
        // TODO: throw error
        return;
    }

    if (y < 0 || y > CHUNK_HEIGHT) {
        // TODO: throw error
        return;
    }

    nodes[index(x, y, z)] = Node{type};
    // printf("Set node at %i, %i, %i to %d\n", x, y, z, type);
}

void Chunk::generateStarterTerrain() {
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int z = 0; z < CHUNK_WIDTH; z++) {
            setNode(x, CHUNK_DEFAULT_GROUND_LEVEL+1, z, GRASS);
            setNode(x, CHUNK_DEFAULT_GROUND_LEVEL, z, STONE);
        }
    }
}
