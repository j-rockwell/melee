//
// Created by John Rockwell on 2/7/25.
//

#ifndef WIREFRAME_H
#define WIREFRAME_H

#include <raylib.h>
#include <raymath.h>

class WireframeHelper {
public:
    static void DrawMeshWireframe(const Mesh &mesh, const Matrix &transform, const Color color) {
        for (int i = 0; i < mesh.triangleCount * 3; i += 3) {
            Vector3 v0 = { mesh.vertices[mesh.indices[i] * 3],
                           mesh.vertices[mesh.indices[i] * 3 + 1],
                           mesh.vertices[mesh.indices[i] * 3 + 2] };
            Vector3 v1 = { mesh.vertices[mesh.indices[i + 1] * 3],
                           mesh.vertices[mesh.indices[i + 1] * 3 + 1],
                           mesh.vertices[mesh.indices[i + 1] * 3 + 2] };
            Vector3 v2 = { mesh.vertices[mesh.indices[i + 2] * 3],
                           mesh.vertices[mesh.indices[i + 2] * 3 + 1],
                           mesh.vertices[mesh.indices[i + 2] * 3 + 2] };

            // Transform the vertices by the translation matrix.
            v0 = Vector3Transform(v0, transform);
            v1 = Vector3Transform(v1, transform);
            v2 = Vector3Transform(v2, transform);

            DrawLine3D(v0, v1, color);
            DrawLine3D(v1, v2, color);
            DrawLine3D(v2, v0, color);
        }
    }
};

#endif //WIREFRAME_H
