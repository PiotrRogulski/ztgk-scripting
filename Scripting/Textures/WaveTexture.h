#pragma once

#include <array>

#include <dxDevice.h>
#include <dxptr.h>

#include "../Geometry/Vector.h"

using namespace mini;
using namespace DirectX;

namespace duck_app {
    class WaveTexture {
    public:
        WaveTexture() = default;

        explicit WaveTexture(const DxDevice& m_device);

        void Update(const DxDevice& m_device) const;

        void SetValue(int x, int y, Vector value);

        static unsigned int GetSize() { return 256; }

        dx_ptr<ID3D11ShaderResourceView> GetTexture(const DxDevice& m_device) const;

    private:
        unsigned int texture_stride;
        unsigned int txture_size;

        std::array<BYTE, 256ULL * 256ULL * 4ULL> texture_data;

        dx_ptr<ID3D11Texture2D> texture;
        dx_ptr<ID3D11ShaderResourceView> texture_resource_view;
    };
}
