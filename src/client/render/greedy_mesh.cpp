//
// Created by John Rockwell on 2/7/25.
//

#include "greedy_mesh.h"
#include "../level/level.h"

/** TODO: this evil must be stopped. rewrite this with whatever AI money can buy **/
Mesh GreedyMesher::generateChunkMesh(const GameLevel* level, const Chunk* chunk) {
    constexpr int dims[3] = { CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH };
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;
    std::vector<unsigned short> indices;
    unsigned int vertexOffset = 0;

    // helper func to get voxel at provided pos
    // if it falls outside the provided chunk we
    // attempt to query the game level
    auto getNodeType = [=](const int lx, const int ly, const int lz) -> int {
        if (lx >= 0 && lx < CHUNK_WIDTH &&
            ly >= 0 && ly < CHUNK_HEIGHT &&
            lz >= 0 && lz < CHUNK_WIDTH) {
            return chunk->getNode(lx, ly, lz).type;
        }

        int globalX = chunk->getChunkX() * CHUNK_WIDTH + lx;
        int globalY = ly;
        int globalZ = chunk->getChunkZ() * CHUNK_WIDTH + lz;
        return level->getNode(globalX, globalY, globalZ).type;
    };

    // loop each axis in the world (0: x, 1: y, 2: z)
    for (int d = 0; d < 3; d++) {
        int u = (d + 1) % 3;
        int v = (d + 2) % 3;
        int maskSize = dims[u] * dims[v];
        int q[3] = {0, 0, 0};
        int x[3] = {0, 0, 0};
        std::vector<int> mask(maskSize, 0);

        q[d] = 1; // direction vector q for the face in this axis

        // loop over d axis from -1 to dims[d]-1
        for (x[d] = -1; x[d] < dims[d]; ) {
            int n = 0;

            // loop over the u and v axes to fill out the mask
            for (x[v] = 0; x[v] < dims[v]; x[v]++) {
                for (x[u] = 0; x[u] < dims[u]; x[u]++) {
                    int aVal = getNodeType(x[0], x[1], x[2]);
                    int bVal = getNodeType(x[0] + q[0], x[1] + q[1], x[2] + q[2]);
                    int a[3] = { x[0], x[1], x[2] };
                    int b[3] = { x[0] + q[0], x[1] + q[1], x[2] + q[2] };

                    if ((aVal != 0) != (bVal != 0)) {
                        // Store a positive value if voxel A is solid, otherwise store a negative value.
                        mask[n] = (aVal != 0) ? aVal : -bVal;
                    } else {
                        mask[n] = 0;
                    }

                    n++;
                }
            }

            x[d]++;

            for (int j = 0; j < dims[v]; j++) {
                for (int i = 0; i < dims[u]; i++) {
                    const int index = i + j * dims[u];

                    if (mask[index] != 0) {
                        const int current = mask[index];

                        // calculate quad width
                        int w;
                        for (w = 1; i + w < dims[u] && mask[index + w] == current; w++) {}

                        // calculate quad height
                        int h;
                        bool done = false;
                        for (h = 1; j + h < dims[v]; h++) {
                            for (int k = 0; k < w; k++) {
                                if (mask[index + k + h * dims[u]] != current) {
                                    done = true;
                                    break;
                                }
                            }

                            if (done) break;
                        }

                        // determine face orientation
                        bool face = (current < 0);
                        int type = (current < 0) ? -current : current;
                        int du[3] = {0, 0, 0};
                        int dv[3] = {0, 0, 0};

                        du[u] = w;
                        dv[v] = h;

                        int pos0[3] = {0, 0, 0};
                        pos0[u] = i;
                        pos0[v] = j;
                        pos0[d] = x[d];

                        // compute the four vertices in chunk-local space
                        Vector3 v0 = { pos0[0] + 0.5f, pos0[1] + 0.5f, pos0[2] + 0.5f };
                        Vector3 v1 = { pos0[0] + du[0] + 0.5f, pos0[1] + du[1] + 0.5f, pos0[2] + du[2] + 0.5f };
                        Vector3 v2 = { pos0[0] + du[0] + dv[0] + 0.5f, pos0[1] + du[1] + dv[1] + 0.5f, pos0[2] + du[2] + dv[2] + 0.5f };
                        Vector3 v3 = { pos0[0] + dv[0] + 0.5f, pos0[1] + dv[1] + 0.5f, pos0[2] + dv[2] + 0.5f };

                        // compute face normal
                        Vector3 normal = { 0, 0, 0 };
                        if (d == 0) normal = { face ? 1.0f : -1.0f, 0.0f, 0.0f };
                        else if (d == 1) normal = { 0.0f, face ? 1.0f : -1.0f, 0.0f };
                        else if (d == 2) normal = { 0.0f, 0.0f, face ? 1.0f : -1.0f };

                        // append 4 vertices
                        float verts[12] = {
                            v0.x, v0.y, v0.z,
                            v1.x, v1.y, v1.z,
                            v2.x, v2.y, v2.z,
                            v3.x, v3.y, v3.z
                        };
                        vertices.insert(vertices.end(), verts, verts + 12);

                        // append normals
                        for (int p = 0; p < 4; p++) {
                            normals.push_back(normal.x);
                            normals.push_back(normal.y);
                            normals.push_back(normal.z);
                        }

                        // append dummy texture coords
                        float uvs[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
                        texcoords.insert(texcoords.end(), uvs, uvs + 8);

                        // append indices for two triangles
                        indices.push_back(vertexOffset + 0);
                        indices.push_back(vertexOffset + 1);
                        indices.push_back(vertexOffset + 2);
                        indices.push_back(vertexOffset + 0);
                        indices.push_back(vertexOffset + 2);
                        indices.push_back(vertexOffset + 3);
                        vertexOffset += 4;

                        // clear mask for quad
                        for (int l = 0; l < h; l++) {
                            for (int k = 0; k < w; k++) {
                                mask[index + k + l * dims[u]] = 0;
                            }
                        }

                        // skip processed cells.
                        i += w;
                    }
                }
            }
        }
    }

    // Create a Raylib Mesh from our arrays.
    Mesh mesh = { 0 };
    mesh.vertexCount = vertices.size() / 3;
    mesh.triangleCount = indices.size() / 3;

    mesh.vertices = new float[vertices.size()];
    memcpy(mesh.vertices, vertices.data(), vertices.size() * sizeof(float));

    mesh.normals = new float[normals.size()];
    memcpy(mesh.normals, normals.data(), normals.size() * sizeof(float));

    mesh.texcoords = new float[texcoords.size()];
    memcpy(mesh.texcoords, texcoords.data(), texcoords.size() * sizeof(float));

    mesh.indices = new unsigned short[indices.size()];
    memcpy(mesh.indices, indices.data(), indices.size() * sizeof(unsigned short));

    return mesh;
}
