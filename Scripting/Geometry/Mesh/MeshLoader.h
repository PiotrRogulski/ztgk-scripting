#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <dxDevice.h>
#include <vertexDef.h>

#include "../Triangle.h"
#include "../UV.h"
#include "../Vector.h"

#include "MeshRobot.h"

using namespace std;

namespace duck_app {
    class MeshLoader {
    public:
        MeshLoader();

        MeshRobot LoadDuck(mini::Mesh& mesh, mini::DxDevice device) const;

        static tuple<vector<mini::VertexPositionNormal>, vector<short>> CreateRectangle(float width, float height);

        static tuple<vector<mini::VertexPositionNormal>, vector<short>> CreateTwoSideRectangle(float width, float height);

        static tuple<vector<mini::VertexPositionNormal>, vector<short>> CreateCylinder(float radius, float height);

        static tuple<vector<mini::VertexPositionNormal>, vector<short>> CreateBox(float width, float height, float depth);

    private:
        vector<string> file_names;

        static MeshRobot LoadSingleMesh(string file_name);

        static void LoadVerticesPositionsNormalsUVs(ifstream& file_stream, vector<Vector>* vertices_positions,
                                                    vector<Vector>* vertices_normals, vector<UV>* uvs);

        static void LoadTriangles(ifstream& file_stream, vector<Triangle>* triangles);

        static vector<short> BoxIndices();

        static vector<mini::VertexPositionNormal> BoxVertices(float width, float height, float depth);
    };
}
