#include "AbsorptionTexture.h"

#include <algorithm>
#include <cmath>

namespace duck_app {
    AbsorptionTexture::AbsorptionTexture() {
        /* Allocate Mamory */
        this->absorption_texture = new float*[this->texture_size];

        for (auto i = 0; i < this->texture_size; i++)
            this->absorption_texture[i] = new float[this->texture_size];

        /* Calculate Distance Data */
        auto half_size_value = this->texture_size / 2.0F;

        for (auto x = 0U; x < this->texture_size; x++)
            for (auto y = 0U; y < this->texture_size; y++) {
                const auto posX = static_cast<float>(x) / static_cast<float>(this->texture_size - 1);
                const auto posY = static_cast<float>(y) / static_cast<float>(this->texture_size - 1);

                auto left = posX;
                auto right = 1.0F - posX;
                auto top = posY;
                auto bottom = 1.0F - posY;

                auto leftDistance = std::max(left, std::max(top, bottom));
                auto rightDistance = std::max(right, std::max(top, bottom));
                auto topDistance = std::max(top, std::max(left, right));
                auto bottomDistance = std::max(bottom, std::max(left, right));

                const auto l = std::min(std::min(leftDistance, rightDistance), std::min(topDistance, bottomDistance));
                const auto absorption_value = 0.95F * std::min(1.0F, l * 5.0F);

                this->absorption_texture[x][y] = absorption_value;
            }
    }

    int AbsorptionTexture::GetSize() const {
        return this->texture_size;
    }

    float AbsorptionTexture::GetValue(int x, int y) const {
        return this->absorption_texture[x][y];
    }
}
