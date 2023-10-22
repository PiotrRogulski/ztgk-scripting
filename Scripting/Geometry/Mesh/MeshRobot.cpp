#include "MeshRobot.h"

namespace duck_app {
    void MeshRobot::AddVertexPosition(const Vector vertex_position) {
        vertices_positions.push_back(vertex_position);
    }

    void MeshRobot::AddVertexPosition(const float x, const float y, const float z) {
        vertices_positions.push_back(Vector(x, y, z));
    }

    void MeshRobot::AddVertex(const int vertex_position_index, const Vector normal_vector) {
        vertices.push_back(vertices_positions[vertex_position_index]);
        normal_vectors.push_back(normal_vector);
    }

    void MeshRobot::AddVertex(const int vertex_position_index, const float x, const float y, const float z) {
        vertices.push_back(vertices_positions[vertex_position_index]);
        normal_vectors.push_back(Vector(x, y, z));
    }

    void MeshRobot::AddTriangle(const Triangle& triangle) {
        triangles.push_back(triangle);
    }

    void MeshRobot::AddTriangle(const int p1, const int p2, const int p3) {
        triangles.push_back(Triangle(p1, p2, p3));
    }

    void MeshRobot::AddEdge(const Edge edge) {
        edges.push_back(edge);
    }

    void MeshRobot::AddEdge(const int v1, const int v2, const int t1, const int t2) {
        edges.push_back(Edge(v1, v2, t1, t2));
    }

    void MeshRobot::AddUV(const UV uv) {
        uvs.push_back(uv);
    }
}
