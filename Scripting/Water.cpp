#include "Water.h"

#include <dxstructures.h>
#include <vertexDef.h>

#include "Geometry/Vector.h"
#include "Geometry/Mesh/MeshLoader.h"

namespace duck_app {
    Water::Water(const DxDevice& m_device,
                 const ConstantBuffer<DirectX::XMFLOAT4X4>& cbWorld,
                 const ConstantBuffer<DirectX::XMFLOAT4X4, 2>& cbView,
                 const ConstantBuffer<DirectX::XMFLOAT4X4>& cbProj,
                 const ConstantBuffer<DirectX::XMFLOAT4X4>& cbTex,
                 const dx_ptr<ID3D11SamplerState>& sampler) : m_random(random_device{}()), wave_texture(m_device) {
        /* Load Mesh */
        MeshLoader mesh_loader;

        auto [vertices, indices] = mesh_loader.CreateRectangle(2.0F, 2.0F);
        water_mesh = m_device.CreateMesh(indices, vertices);

        XMStoreFloat4x4(&water_mesh_matrix, XMMatrixScaling(5, 1, 5));

        /* Load Shaders */
        const auto vsCode = m_device.LoadByteCode(L"WaterVertexShader.cso");
        const auto psCode = m_device.LoadByteCode(L"WaterPixelShader.cso");

        //dx_ptr<ID3D11ShaderResourceView> m_envTexture = m_device.CreateShaderResourceView(L"Resources/Textures/Colours.dds");
        const auto m_envTexture = m_device.CreateShaderResourceView(
            L"Resources/Textures/CubeTex/skybox_autum_forest.dds");

        /* Textures */
        height_map_texture_1 = &this->height_map_textures[0];
        height_map_texture_2 = &this->height_map_textures[1];
        height_map_texture_3 = &this->height_map_textures[2];

        /* Create Effect */
        water_effect = MultiTexturedEffect(m_device.CreateVertexShader(vsCode), m_device.CreatePixelShader(psCode),
                                           cbWorld, cbView, cbProj, cbTex, cbTex, sampler, m_envTexture, m_envTexture);

        /* SetUp Rasterizers States */
        mini::utils::RasterizerDescription rasterizer_desc;
        rasterizer_desc.CullMode = D3D11_CULL_NONE;

        m_rsCullNone = m_device.CreateRasterizerState(rasterizer_desc);
    }

    void Water::Update(const DxDevice& m_device) {
        this->UpdateHeightMap();
        this->UpdateWaveTextureMap(m_device);
        this->GetRandomDrop();

        water_effect.SetTexture2(this->wave_texture.GetTexture(m_device));
    }

    void Water::Render(const DxDevice& m_device, ConstantBuffer<XMFLOAT4X4>* m_cbWorldMtx, const ConstantBuffer<XMFLOAT4>& m_camPos) {
        m_device.context()->RSSetState(m_rsCullNone.get());

        water_effect.SetCameraPosition(m_camPos);
        water_effect.Begin(m_device.context());

        m_cbWorldMtx->Update(m_device.context(), water_mesh_matrix);
        water_mesh.Render(m_device.context());

        m_device.context()->RSSetState(nullptr);
    }

    void Water::CreateRipple(const int x, const int y) const {
        this->height_map_texture_1->SetValue(x, y, 2.25F);
    }

    void Water::GetRandomDrop() {
        static uniform_real_distribution<float> random(0, 100);

        if (random(m_random) > 65.0f) {
            static uniform_real_distribution<float> random_x(0, 256);
            static uniform_real_distribution<float> random_y(0, 256);

            this->CreateRipple(static_cast<int>(random_x(m_random)), static_cast<int>(random_y(m_random)));
        }
    }

    void Water::UpdateWithDuckTrace(const XMFLOAT3 duck_position) const {
        const auto x = static_cast<int>(duck_position.x + 2.5) * 255 / 5;
        const auto y = static_cast<int>(duck_position.z + 2.5) * 255 / 5;

        if (x < 0 || x > 255 || y < 0 || y > 255)
            return;

        this->CreateRipple(x, y);
    }

    void Water::UpdateHeightMap() {
        const auto tmp_height_map_texture = this->height_map_texture_3;
        this->height_map_texture_3 = this->height_map_texture_2;
        this->height_map_texture_2 = this->height_map_texture_1;
        this->height_map_texture_1 = tmp_height_map_texture;

        const auto texture_size = this->height_map_texture_1->GetSize();

        constexpr auto wave_speed = 1.0F;
        const auto integration_step = 1.0F / static_cast<float>(texture_size);


        const auto h_value = 2.0F / static_cast<float>(texture_size - 1);
        const auto a_coef = wave_speed * wave_speed * integration_step * integration_step / (h_value * h_value);
        const auto b_coef = 2.0F - 4.0F * a_coef;

        /* Calculate New Values For HeightMap */
        for (auto y = 0; y < texture_size; ++y) {
            for (auto x = 0; x < texture_size; ++x) {
                const auto lastX = x > 0 ? x - 1 : 0;
                const auto nextX = x < texture_size - 1 ? x + 1 : texture_size - 1;

                const auto lastY = y > 0 ? y - 1 : 0;
                const auto nextY = y < texture_size - 1 ? y + 1 : texture_size - 1;

                const auto absorption_value = this->absorption_texture.GetValue(x, y);
                const auto value = absorption_value * (
                                       a_coef * (this->height_map_texture_2->GetValue(nextX, y) + this->height_map_texture_2
                                                                                                      ->GetValue(lastX, y) + this->height_map_texture_2->GetValue(x, lastY) + this
                                                                                                                                                                              ->height_map_texture_2->GetValue(x, nextY)) + b_coef * this->
                                                                                                                                                                                                                                     height_map_texture_2->GetValue(x, y) - this->height_map_texture_3->GetValue(x, y));

                this->height_map_texture_1->SetValue(x, y, value);
            }
        }
    }

    void Water::UpdateWaveTextureMap(const DxDevice& m_device) {
        const int texture_size = this->wave_texture.GetSize();

        for (auto y = 0; y < texture_size; ++y) {
            for (auto x = 0; x < texture_size; ++x) {
                const auto prevX = (x + texture_size - 1) % texture_size;
                const auto nextX = (x + 1) % texture_size;
                const auto prevY = (y + texture_size - 1) % texture_size;
                const auto nextY = (y + 1) % texture_size;

                const auto p00 = this->height_map_texture_1->GetValue(prevX, prevY);
                const auto p10 = this->height_map_texture_1->GetValue(x, prevY);
                const auto p20 = this->height_map_texture_1->GetValue(nextX, prevY);

                const auto p01 = this->height_map_texture_1->GetValue(prevX, y);
                const auto p11 = this->height_map_texture_1->GetValue(x, y);
                const auto p21 = this->height_map_texture_1->GetValue(nextX, y);

                const auto p02 = this->height_map_texture_1->GetValue(prevX, nextY);
                const auto p12 = this->height_map_texture_1->GetValue(x, nextY);
                const auto p22 = this->height_map_texture_1->GetValue(nextX, nextY);

                const auto horizontal = (p01 - p21) * 2.0F + p20 + p22 - p00 - p02;
                const auto vertical = (p12 - p10) * 2.0F + p22 + p02 - p20 - p00;
                constexpr auto depth = 1.0F / 2.0F;

                auto r = horizontal;
                auto g = vertical;
                auto b = depth;

                const auto length = std::sqrt(r * r + g * g + b * b);
                r /= length;
                g /= length;
                b /= length;

                this->wave_texture.SetValue(x, y, Vector(r, g, b));
            }
        }
        this->wave_texture.Update(m_device);
    }
}
