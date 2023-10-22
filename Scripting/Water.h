#pragma once

#include <constantBuffer.h>
#include <mesh.h>

#include <array>
#include <random>

#include <dxptr.h>
#include <effect.h>

#include "Textures/AbsorptionTexture.h"
#include "Textures/HeightMapTexture.h"
#include "Textures/WaveTexture.h"

#include "Gk2BaseClasses/MultiTexturedEffect.h"

#include <DirectXMath.h>

using namespace std;
using namespace mini;
using namespace DirectX;

namespace duck_app {
    class Water {
    public:
        Water() = default;

        Water(const DxDevice& m_device,
              const ConstantBuffer<XMFLOAT4X4>& cbWorld,
              const ConstantBuffer<XMFLOAT4X4, 2>& cbView,
              const ConstantBuffer<XMFLOAT4X4>& cbProj,
              const ConstantBuffer<XMFLOAT4X4>& cbTex,
              const dx_ptr<ID3D11SamplerState>& sampler);

        void Update(const DxDevice& m_device);

        void Render(const DxDevice& m_device, ConstantBuffer<XMFLOAT4X4>* m_cbWorldMtx,
                    const ConstantBuffer<XMFLOAT4>& m_camPos);

        void UpdateWithDuckTrace(XMFLOAT3 duck_position) const;

    private:
        MultiTexturedEffect water_effect;
        dx_ptr<ID3D11RasterizerState> m_rsCullNone;

        Mesh water_mesh;
        XMFLOAT4X4 water_mesh_matrix;

        default_random_engine m_random;

        WaveTexture wave_texture;
        AbsorptionTexture absorption_texture;
        std::array<HeightMapTexture, 3> height_map_textures;
        HeightMapTexture* height_map_texture_1; /* Actual (n+1) Value of HeightMap */
        HeightMapTexture* height_map_texture_2; /* Previous (n) Value of HeightMap */
        HeightMapTexture* height_map_texture_3; /* Previous (n-1) Value of HeightMap */

        void CreateRipple(int x, int y) const;

        void GetRandomDrop();

        void UpdateHeightMap();

        void UpdateWaveTextureMap(const DxDevice& m_device);
    };
}
