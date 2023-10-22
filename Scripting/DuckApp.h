#pragma once
#include <memory>

#include <constantBuffer.h>
#include <DirectXMath.h>
#include <mesh.h>

#include "Gk2BaseClasses/Gk2ExampleBase.h"
#include "Gk2BaseClasses/TexturedEffect.h"

#include "GameObject.h"
#include "Water.h"

/* TODO (4.01) Include Script Manager */

using namespace mini;

namespace duck_app {
    class DuckApp: public Gk2ExampleBase {
    public:
        explicit DuckApp(HINSTANCE hInstance);

    protected:
        void Update(const Clock& clock) override;

        void Render() override;

    private:
        static const XMFLOAT4 LIGHT_POS;

        /* TODO (4.02) Create Script Manager Pointer Instance */


        std::unique_ptr<GameObject> duck_instance;
        std::unique_ptr<Water> water_instance;

        Mesh wall_mesh;
        XMFLOAT4X4 wall_mesh_matrix[6];


        XMFLOAT4X4 m_projMtx;
        dx_ptr<ID3D11InputLayout> m_inputlayout;
        dx_ptr<ID3D11SamplerState> m_samplerWrap;
        dx_ptr<ID3D11SamplerState> m_samplerBorder;


        ConstantBuffer<XMFLOAT4X4>
            m_cbWorldMtx, //vertex shader constant buffer slot 0
            m_cbProjMtx;  //vertex shader constant buffer slot 2 & geometry shader constant buffer slot 0

        ConstantBuffer<XMFLOAT4> m_cbCameraPos;    //vertex shader constant buffer for Camera Position for Water
        ConstantBuffer<XMFLOAT4> m_cbLightPos;     //pixel shader constant buffer slot 1
        ConstantBuffer<XMFLOAT4X4, 2> m_cbViewMtx; //vertex shader constant buffer slot 1
        ConstantBuffer<XMFLOAT4> m_cbSurfaceColor; //pixel shader constant buffer slot 0
        ConstantBuffer<XMFLOAT4X4> m_cbTexMtx;

        void DrawWalls();

        void DrawDuck();

        void DrawWater();

        Mesh box_mesh;
        XMFLOAT4X4 box_mesh_matrix;

        TexturedEffect textured_wall_effect;

        void UpdateCamera(const XMMATRIX& viewMtx);

        void UpdateDuck(float dtime) const;

        void UpdateWater() const;

        void SetWorldMtx(const XMFLOAT4X4& mtx);

        void DrawMesh(const Mesh& m, const XMFLOAT4X4& worldMtx);

        void SetSurfaceColor(XMFLOAT4 color);
    };
}
