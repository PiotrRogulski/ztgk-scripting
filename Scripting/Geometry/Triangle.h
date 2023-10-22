#pragma once

#include <vector>

#include <DirectXMath.h>

#include "Vector.h"

namespace duck_app {
    struct Triangle {
        short vertex_indices[3];

        DirectX::XMVECTOR vector_1;
        DirectX::XMVECTOR vector_2;
        DirectX::XMVECTOR vector_3;

        DirectX::XMVECTOR normal;

        Triangle();

        Triangle(short p1, short p2, short p3);

        void SetXMVECTORs(const std::vector<Vector>& positions);

        //DirectX::XMVECTOR GetNormal(std::vector<Vector> positions);
        [[nodiscard]] DirectX::XMVECTOR GetLightVector(std::vector<Vector> positions, DirectX::XMVECTOR light_position) const;

        bool IsBackFace(const std::vector<Vector>& positions, const DirectX::XMMATRIX& model_matrix,
                        DirectX::XMVECTOR light_position) const;
    };
}
