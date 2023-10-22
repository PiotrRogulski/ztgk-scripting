#include "WaveTexture.h"

#include <dxstructures.h>

using namespace mini::utils;

namespace duck_app {
    WaveTexture::WaveTexture(const DxDevice& m_device) {
        constexpr auto texture_bpp = 4U;
        constexpr auto texture_width = 256U;
        constexpr auto texture_height = 256U;

        texture_stride = texture_width * texture_bpp;
        txture_size = texture_stride * texture_height;

        auto texture_description = Texture2DDescription(texture_width, texture_height);

        texture_description.BindFlags |= D3D11_BIND_RENDER_TARGET;
        texture_description.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        this->texture = m_device.CreateTexture(texture_description);
        this->texture_resource_view = m_device.CreateShaderResourceView(this->texture);

        auto value_pointer = this->texture_data.data();
    }

    void WaveTexture::SetValue(const int x, const int y, const Vector value) {
        auto value_pointer = this->texture_data.data();

        value_pointer += 256 * 4 * y + 4 * x;

        *value_pointer++ = static_cast<BYTE>(255 * (value.x + 1) / 2);
        *value_pointer++ = static_cast<BYTE>(255 * (value.y + 1) / 2);
        *value_pointer++ = static_cast<BYTE>(255 * (value.z + 1) / 2);
        *value_pointer++ = static_cast<BYTE>(255);
    }

    void WaveTexture::Update(const DxDevice& m_device) const {
        m_device.context()->UpdateSubresource(this->texture.get(), 0, nullptr, texture_data.data(), texture_stride,
                                              txture_size);
    }

    dx_ptr<ID3D11ShaderResourceView> WaveTexture::GetTexture(const DxDevice& m_device) const {
        return m_device.CreateShaderResourceView(this->texture);
    }
}
