#pragma once

#include <vector>

#include <DirectXMath.h>
#include <mesh.h>
#include <constantBuffer.h>
#include <dxptr.h>

#include "Geometry/Mesh/MeshRobot.h"

#include "Gk2BaseClasses/TexturedEffect.h"

#include "Script.h"


using namespace std;
using namespace mini;
using namespace DirectX;

/* TODO (3.01) Include Python And PyBind11 Headers */


namespace duck_app {
    struct DuckLayout {
        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT2 uv;

        static const D3D11_INPUT_ELEMENT_DESC Layout[3];

        DuckLayout() : position(0.0F, 0.0F, 0.0F), normal(0.0F, 0.0F, 0.0F), uv(0.0F, 0.0F) { }
    };

    class GameObject /* Represents Duck Instance */
    {
    public:
        GameObject() = default;

        GameObject(const DxDevice& m_device,
                   const ConstantBuffer<XMFLOAT4X4>& cbWorld,
                   const ConstantBuffer<XMFLOAT4X4, 2>& cbView,
                   const ConstantBuffer<XMFLOAT4X4>& cbProj,
                   const ConstantBuffer<XMFLOAT4X4>& cbTex,
                   const dx_ptr<ID3D11SamplerState>& sampler);

        void Update(DxDevice m_device, float dtime);

        void Render(const DxDevice& m_device, ConstantBuffer<XMFLOAT4X4>* m_cbWorldMtx,
                    const ConstantBuffer<XMFLOAT4>& camera_position, const ConstantBuffer<XMFLOAT4>& light_position);

        /* TODO (3.03) Define Methods For Script Management */
        /*
            * Returns Script Associated With This Game Object (Duck)
        */
        Script* GetScript() const { return this->script.get(); }

        /* TODO (3.04) Define Script Methods */
        /*
            * On Create Method
            * On Update Method
        */
        void OnCreate();

        void OnUpdate();

        /* TODO (3.05) Define C++ API Methods */
        /*
            * Returns Next Duck Position
            * Returns Current Duck Scale
            * Returns Current Duck Position

            * Sets Next Duck Position
            * Sets Current Duck Scale
            * Sets Current Duck Position
        */
        XMFLOAT3 GetPosition() const { return this->current_position; }

        XMFLOAT3 GetScale() const { return this->current_scale; }

        XMFLOAT3 GetNextPosition() const { return this->next_position; }

        void SetPosition(const XMFLOAT3 newPosition) { this->current_position = newPosition; }

        void SetScale(const XMFLOAT3 newScale) { this->current_scale = newScale; }

        void SetNextPosition(const XMFLOAT3 nextPosition) { this->next_position = nextPosition; }

    private:
        /* TODO (3.02) Add Script Pointer Instance */
        std::unique_ptr<Script> script;

        XMFLOAT3 current_scale = {0.1F, 0.1F, 0.1F};
        XMFLOAT3 next_position = {1, 0, 0};
        XMFLOAT3 current_position = {0, 0, 0};
        XMFLOAT4X4 current_rotation;

        Mesh duck_mesh;
        XMFLOAT4X4 duck_mesh_matrix;

        vector<short> indices;
        vector<DuckLayout> duck_layout;
        dx_ptr<ID3D11Buffer> m_vertices;
        dx_ptr<ID3D11Buffer> m_indices;

        static const unsigned int OFFSET;
        static const unsigned int STRIDE;

        TexturedEffect duck_textured_effect;
        dx_ptr<ID3D11InputLayout> m_inputLayout;

        void LoadDuckLayout(const DxDevice& m_device, const MeshRobot& duck_mesh_info);
    };
}
