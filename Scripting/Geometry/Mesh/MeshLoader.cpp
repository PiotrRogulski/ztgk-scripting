#include "MeshLoader.h"

#include <DirectXMath.h>
#include <fstream>
#include <mesh.h>

using namespace std;
using namespace DirectX;

namespace duck_app {
    MeshLoader::MeshLoader() {
        file_names.push_back("Duck_Mesh/duck.txt");
    }

    MeshRobot MeshLoader::LoadDuck(mini::Mesh& mesh, mini::DxDevice device) const {
        auto mesh_robot = LoadSingleMesh(file_names[0]);

        vector<short> indices;
        vector<mini::VertexPositionNormal> vertices;

        for (auto i = 0; i < mesh_robot.triangles.size(); i++) {
            auto t = mesh_robot.triangles[i];

            indices.push_back(t.vertex_indices[0]);
            indices.push_back(t.vertex_indices[1]);
            indices.push_back(t.vertex_indices[2]);
        }

        for (auto i = 0; i < mesh_robot.vertices.size(); i++) {
            vertices.push_back({
                XMFLOAT3
                {
                    mesh_robot.vertices[i].x,
                    mesh_robot.vertices[i].y,
                    mesh_robot.vertices[i].z
                },
                XMFLOAT3
                {
                    mesh_robot.normal_vectors[i].x,
                    mesh_robot.normal_vectors[i].y,
                    mesh_robot.normal_vectors[i].z
                }
            });
        }

        mesh = device.CreateMesh(indices, vertices);

        return mesh_robot;
    }

    MeshRobot MeshLoader::LoadSingleMesh(string file_name) {
        MeshRobot mesh_robot;

        vector<int> vertices_indices;
        vector<UV> uvs;
        vector<Vector> vertices_normals;
        vector<Vector> vertices_positions;
        vector<Triangle> triangles_mesh;
        vector<Edge> edges;

        ifstream input_mesh_file;
        input_mesh_file.open(file_name);

        if (input_mesh_file.good()) {
            LoadVerticesPositionsNormalsUVs(input_mesh_file, &vertices_positions, &vertices_normals, &uvs);

            for (auto i = 0; i < vertices_positions.size(); i++)
                mesh_robot.AddVertexPosition(vertices_positions[i]);

            for (auto i = 0; i < vertices_normals.size(); i++)
                mesh_robot.AddVertex(i, vertices_normals[i]);

            for (auto i = 0; i < uvs.size(); i++)
                mesh_robot.AddUV(uvs[i]);

            LoadTriangles(input_mesh_file, &triangles_mesh);

            for (auto i = 0; i < triangles_mesh.size(); i++)
                mesh_robot.AddTriangle(triangles_mesh[i]);
        }

        return mesh_robot;
    }

    void MeshLoader::LoadVerticesPositionsNormalsUVs(ifstream& file_stream, vector<Vector>* vertices_positions,
                                                     vector<Vector>* vertices_normals, vector<UV>* uvs) {
        int vertices_normals_uv_count;
        file_stream >> vertices_normals_uv_count;

        for (auto i = 0; i < vertices_normals_uv_count; i++) {
            float vertex_x;
            float vertex_y;
            float vertex_z;

            file_stream >> vertex_x >> vertex_y >> vertex_z;
            vertices_positions->push_back(Vector(vertex_x, vertex_y, vertex_z));


            float normal_x;
            float normal_y;
            float normal_z;

            file_stream >> normal_x >> normal_y >> normal_z;
            vertices_normals->push_back(Vector(normal_x, normal_y, normal_z));


            float texture_u;
            float texture_v;

            file_stream >> texture_u >> texture_v;
            uvs->push_back(UV(texture_u, texture_v));
        }
    }

    void MeshLoader::LoadTriangles(ifstream& file_stream, vector<Triangle>* triangles) {
        int triangles_lines_count;
        file_stream >> triangles_lines_count;

        for (auto i = 0; i < triangles_lines_count; i++) {
            int p1;
            int p2;
            int p3;

            file_stream >> p1 >> p2 >> p3;
            triangles->push_back(Triangle(p1, p2, p3));
        }
    }

#pragma region Box Mesh
    tuple<vector<mini::VertexPositionNormal>, vector<short>> MeshLoader::CreateBox(
        const float width, const float height, const float depth) {
        auto indices = BoxIndices();
        std::reverse(indices.begin(), indices.end());
        return {BoxVertices(width, height, depth), indices};
    }

    vector<mini::VertexPositionNormal> MeshLoader::BoxVertices(const float width, const float height, const float depth) {
        return {
            //Front face
            {XMFLOAT3(-0.5F * width, -0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, 0.0F, 1.0F)},
            {XMFLOAT3(+0.5F * width, -0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, 0.0F, 1.0F)},
            {XMFLOAT3(+0.5F * width, +0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, 0.0F, 1.0F)},
            {XMFLOAT3(-0.5F * width, +0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, 0.0F, 1.0F)},

            //Back face
            {XMFLOAT3(+0.5F * width, -0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, 0.0F, -1.0F)},
            {XMFLOAT3(-0.5F * width, -0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, 0.0F, -1.0F)},
            {XMFLOAT3(-0.5F * width, +0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, 0.0F, -1.0F)},
            {XMFLOAT3(+0.5F * width, +0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, 0.0F, -1.0F)},

            //Left face
            {XMFLOAT3(-0.5F * width, -0.5F * height, +0.5F * depth), XMFLOAT3(1.0F, 0.0F, 0.0F)},
            {XMFLOAT3(-0.5F * width, -0.5F * height, -0.5F * depth), XMFLOAT3(1.0F, 0.0F, 0.0F)},
            {XMFLOAT3(-0.5F * width, +0.5F * height, -0.5F * depth), XMFLOAT3(1.0F, 0.0F, 0.0F)},
            {XMFLOAT3(-0.5F * width, +0.5F * height, +0.5F * depth), XMFLOAT3(1.0F, 0.0F, 0.0F)},

            //Right face
            {XMFLOAT3(+0.5F * width, -0.5F * height, -0.5F * depth), XMFLOAT3(-1.0F, 0.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, -0.5F * height, +0.5F * depth), XMFLOAT3(-1.0F, 0.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, +0.5F * height, +0.5F * depth), XMFLOAT3(-1.0F, 0.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, +0.5F * height, -0.5F * depth), XMFLOAT3(-1.0F, 0.0F, 0.0F)},

            //Bottom face
            {XMFLOAT3(-0.5F * width, -0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, 1.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, -0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, 1.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, -0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, 1.0F, 0.0F)},
            {XMFLOAT3(-0.5F * width, -0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, 1.0F, 0.0F)},

            //Top face
            {XMFLOAT3(-0.5F * width, +0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, -1.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, +0.5F * height, -0.5F * depth), XMFLOAT3(0.0F, -1.0F, 0.0F)},
            {XMFLOAT3(+0.5F * width, +0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, -1.0F, 0.0F)},
            {XMFLOAT3(-0.5F * width, +0.5F * height, +0.5F * depth), XMFLOAT3(0.0F, -1.0F, 0.0F)},
        };
    }

    vector<short> MeshLoader::BoxIndices() {
        return {
            0,
            2,
            1,
            0,
            3,
            2,
            4,
            6,
            5,
            4,
            7,
            6,
            8,
            10,
            9,
            8,
            11,
            10,
            12,
            14,
            13,
            12,
            15,
            14,
            16,
            18,
            17,
            16,
            19,
            18,
            20,
            22,
            21,
            20,
            23,
            22
        };
    }
#pragma endregion

#pragma region Rectangle Mesh
    tuple<vector<mini::VertexPositionNormal>, vector<short>> MeshLoader::CreateRectangle(const float width, const float height) {
        return
        {
            {
                {XMFLOAT3(-0.5f * width, 0.0f, -0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f)},
                {XMFLOAT3(-0.5f * width, 0.0f, +0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f)},
                {XMFLOAT3(+0.5f * width, 0.0f, +0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f)},
                {XMFLOAT3(+0.5f * width, 0.0f, -0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f)}
            },
            {
                0,
                1,
                2,
                0,
                2,
                3
            }
        };
    }

    tuple<vector<mini::VertexPositionNormal>, vector<short>> MeshLoader::CreateTwoSideRectangle(
        const float width, const float height) {
        return
        {
            {
                {XMFLOAT3(-0.5f * width, -0.5f * height, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
                {XMFLOAT3(-0.5f * width, +0.5f * height, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
                {XMFLOAT3(+0.5f * width, +0.5f * height, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
                {XMFLOAT3(+0.5f * width, -0.5f * height, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)}
            },
            {
                0,
                1,
                2,
                0,
                2,
                3,
                3,
                2,
                0,
                2,
                1,
                0
            }
        };
    }
#pragma endregion

#pragma region Cylinder Mesh
    tuple<vector<mini::VertexPositionNormal>, vector<short>> MeshLoader::CreateCylinder(const float radius, const float height) {
        constexpr auto count = 100;
        constexpr auto rStep = XM_2PI / 100;
        const auto h = height / 2;
        vector<mini::VertexPositionNormal> points;
        vector<short> ind;

        for (auto i = 0; i < count; i++) {
            const auto alpha = rStep * i;
            const auto n = XMFLOAT3(cos(alpha), 0, sin(alpha));
            points.push_back({XMFLOAT3(radius * cos(alpha), -h, radius * sin(alpha)), n});
            points.push_back({XMFLOAT3(radius * cos(alpha), h, radius * sin(alpha)), n});
        }

        for (auto i = 0; i < count; i++) {
            const auto alpha = rStep * i;
            points.push_back({XMFLOAT3(radius * cos(alpha), h, radius * sin(alpha)), XMFLOAT3(0, 1, 0)});
        }

        for (auto i = 0; i < count; i++) {
            const auto alpha = rStep * i;
            points.push_back({XMFLOAT3(radius * cos(alpha), -h, radius * sin(alpha)), XMFLOAT3(0, -1, 0)});
        }

        points.push_back({XMFLOAT3(0, h, 0), XMFLOAT3(0, 1, 0)});
        points.push_back({XMFLOAT3(0, -h, 0), XMFLOAT3(0, -1, 0)});

        for (auto i = 0; i < count - 1; i++) {
            ind.push_back(2 * i);
            ind.push_back(2 * i + 1);
            ind.push_back(2 * (i + 1));

            ind.push_back(2 * (i + 1));
            ind.push_back(2 * i + 1);
            ind.push_back(2 * i + 3);
        }

        ind.push_back(2 * count - 2);
        ind.push_back(2 * count - 1);
        ind.push_back(0);

        ind.push_back(0);
        ind.push_back(2 * count - 1);
        ind.push_back(1);

        for (auto i = 2 * count; i < 3 * count - 1; i++) {
            ind.push_back(i + 1);
            ind.push_back(i);
            ind.push_back(static_cast<short>(points.size()) - 2);
        }

        ind.push_back(2 * count);
        ind.push_back(3 * count - 1);
        ind.push_back(static_cast<short>(points.size()) - 2);

        for (auto i = 3 * count; i < 4 * count - 1; i++) {
            ind.push_back(static_cast<short>(points.size()) - 1);
            ind.push_back(i);
            ind.push_back(i + 1);
        }

        ind.push_back(static_cast<short>(points.size()) - 1);
        ind.push_back(4 * count - 1);
        ind.push_back(3 * count);

        return {points, ind};
    }
#pragma endregion
}
