#include "GameObject.h"

#include "Geometry/Mesh/MeshLoader.h"

/* TODO (3.06) Include Python And PyBind11 Headers */


namespace duck_app {
    const D3D11_INPUT_ELEMENT_DESC DuckLayout::Layout[3] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    const unsigned int GameObject::STRIDE = sizeof(DuckLayout);
    const unsigned int GameObject::OFFSET = 0;

    GameObject::GameObject(const DxDevice& m_device,
                           const ConstantBuffer<XMFLOAT4X4>& cbWorld,
                           const ConstantBuffer<XMFLOAT4X4, 2>& cbView,
                           const ConstantBuffer<XMFLOAT4X4>& cbProj,
                           const ConstantBuffer<XMFLOAT4X4>& cbTex,
                           const dx_ptr<ID3D11SamplerState>& sampler) {
        const MeshLoader mesh_loader;
        const auto duck_mesh_with_UV = mesh_loader.LoadDuck(duck_mesh, m_device);
        XMStoreFloat4x4(&duck_mesh_matrix, XMMatrixIdentity());

        this->LoadDuckLayout(m_device, duck_mesh_with_UV);

        const auto vsCode = m_device.LoadByteCode(L"DuckTexVertexShader.cso");
        const auto psCode = m_device.LoadByteCode(L"DuckTexPixelShader.cso");

        const auto m_surfaceTexture = m_device.CreateShaderResourceView(L"Duck_Mesh/ducktex.jpg");

        duck_textured_effect = TexturedEffect(m_device.CreateVertexShader(vsCode), m_device.CreatePixelShader(psCode),
                                              cbWorld, cbView, cbProj, cbTex, sampler, m_surfaceTexture);

        m_inputLayout = m_device.CreateInputLayout(DuckLayout::Layout, vsCode);


        /* TODO (3.07) Create New Script With Specified Name And Initialize It */
        this->script = std::make_unique<Script>("Resources/Scripts/DuckScript.py");
        this->script->Initialize(this);
    }

    void GameObject::LoadDuckLayout(const DxDevice& m_device, const MeshRobot& duck_mesh_info) {
        for (auto i = 0; i < duck_mesh_info.vertices_positions.size(); i++) {
            DuckLayout layout;

            const auto position = duck_mesh_info.vertices_positions[i];
            const auto normal = duck_mesh_info.normal_vectors[i];
            const auto uv = duck_mesh_info.uvs[i];

            layout.position = XMFLOAT3(position.x, position.y, position.z);
            layout.normal = XMFLOAT3(normal.x, normal.y, normal.z);
            layout.uv = XMFLOAT2(uv.u, uv.v);

            duck_layout.push_back(layout);
        }

        for (auto i = 0; i < duck_mesh_info.triangles.size(); i++) {
            indices.push_back(duck_mesh_info.triangles[i].vertex_indices[0]);
            indices.push_back(duck_mesh_info.triangles[i].vertex_indices[1]);
            indices.push_back(duck_mesh_info.triangles[i].vertex_indices[2]);
        }

        m_indices = m_device.CreateIndexBuffer(indices);
        m_vertices = m_device.CreateVertexBuffer<DuckLayout>(static_cast<UINT>(duck_layout.size()));

        D3D11_MAPPED_SUBRESOURCE resource;
        auto hr = m_device.context()->Map(m_vertices.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

        memcpy(resource.pData, duck_layout.data(), duck_layout.size() * sizeof(DuckLayout));
        m_device.context()->Unmap(m_vertices.get(), 0);
    }

    void GameObject::Render(const DxDevice& m_device, ConstantBuffer<XMFLOAT4X4>* m_cbWorldMtx,
                            const ConstantBuffer<XMFLOAT4>& camera_position,
                            const ConstantBuffer<XMFLOAT4>& light_position) {
        XMStoreFloat4x4(&duck_mesh_matrix,
                        XMMatrixScaling(this->current_scale.x, this->current_scale.y, this->current_scale.z) *
                        XMLoadFloat4x4(&this->current_rotation) *
                        XMMatrixTranslation(this->current_position.x, this->current_position.y,
                                            this->current_position.z));

        /* Set Duck Effect */
        duck_textured_effect.SetCameraPosition(camera_position);
        duck_textured_effect.SetLightPosition(light_position);

        duck_textured_effect.Begin(m_device.context());

        /* Update Model Matrix */
        m_cbWorldMtx->Update(m_device.context(), duck_mesh_matrix);

        /* Create and Pass Buffers to Shaders */
        ID3D11Buffer* vb[1] = {m_vertices.get()};

        m_device.context()->IASetIndexBuffer(m_indices.get(), DXGI_FORMAT_R16_UINT, 0);
        m_device.context()->IASetVertexBuffers(0, 1, vb, &STRIDE, &OFFSET);
        m_device.context()->IASetInputLayout(m_inputLayout.get());
        m_device.context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        /* Render Duck */
        m_device.context()->DrawIndexed(static_cast<int>(indices.size()), 0U, 0U);

        /* CleanUp Made Outside */
    }

    void GameObject::Update(DxDevice m_device, float dtime) {
        constexpr auto up_float3 = XMFLOAT3(0.0F, 1.0F, 0.0F);
        const auto up_vector = XMLoadFloat3(&up_float3);

        auto rotation_matrix = XMMatrixLookAtRH(XMLoadFloat3(&this->next_position),
                                                XMLoadFloat3(&this->current_position), up_vector);
        rotation_matrix = XMMatrixInverse(nullptr, rotation_matrix);

        XMStoreFloat4x4(&this->current_rotation, XMMatrixRotationY(XM_PIDIV2) * rotation_matrix);
    }

    /* TODO (3.08) Implement OnCreate And OnUpdate Methods (Should Call Methods From Script) */
}
