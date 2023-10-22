#include "Triangle.h"

using namespace std;
using namespace DirectX;

namespace duck_app {
    Triangle::Triangle() {
        vertex_indices[0] = 0;
        vertex_indices[1] = 0;
        vertex_indices[2] = 0;
    }

    Triangle::Triangle(const short p1, const short p2, const short p3) {
        vertex_indices[0] = p1;
        vertex_indices[1] = p2;
        vertex_indices[2] = p3;
    }

    void Triangle::SetXMVECTORs(const std::vector<Vector>& positions) {
        const auto triangle_1_vector_0 = positions[vertex_indices[0]];
        const auto triangle_1_vector_1 = positions[vertex_indices[1]];
        const auto triangle_1_vector_2 = positions[vertex_indices[2]];

        const auto triangle_1_float3_0 = XMFLOAT3(triangle_1_vector_0.x, triangle_1_vector_0.y, triangle_1_vector_0.z);
        const auto triangle_1_float3_1 = XMFLOAT3(triangle_1_vector_1.x, triangle_1_vector_1.y, triangle_1_vector_1.z);
        const auto triangle_1_float3_2 = XMFLOAT3(triangle_1_vector_2.x, triangle_1_vector_2.y, triangle_1_vector_2.z);

        vector_1 = XMLoadFloat3(&triangle_1_float3_0);
        vector_2 = XMLoadFloat3(&triangle_1_float3_1);
        vector_3 = XMLoadFloat3(&triangle_1_float3_2);

        const auto triangle_1_U = vector_2 - vector_1;
        const auto triangle_1_V = vector_3 - vector_1;

        normal = XMVector3Cross(triangle_1_U, triangle_1_V);
    }

    //XMVECTOR Triangle::GetNormal(vector<Vector> positions)
    //{
    //    XMVECTOR triangle_1_U = vector_2 - vector_1;
    //    XMVECTOR triangle_1_V = vector_3 - vector_1;

    //    return XMVector3Cross(triangle_1_U, triangle_1_V);
    //}

    XMVECTOR Triangle::GetLightVector(vector<Vector> positions, const XMVECTOR light_position) const {
        return ((vector_1 + vector_2 + vector_3) / 3.0F) - light_position;
    }

    bool Triangle::IsBackFace(const vector<Vector>& positions, const XMMATRIX& model_matrix,
                              const XMVECTOR light_position) const {
        return XMVectorGetX(XMVector3Dot(XMVector3TransformNormal(normal, model_matrix),
                                         GetLightVector(positions, light_position))) > 0;
    }
}
