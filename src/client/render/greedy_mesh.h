//
// Created by John Rockwell on 2/7/25.
//

#ifndef GREEDY_MESH_H
#define GREEDY_MESH_H

#include <raylib.h>
#include "../level/chunk.h"

class GreedyMesher {
public:
    static Mesh generateChunkMesh(const Chunk* chunk);
};

#endif //GREEDY_MESH_H
