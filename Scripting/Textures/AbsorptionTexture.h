#pragma once

namespace duck_app {
    class AbsorptionTexture {
    public:
        AbsorptionTexture();

        [[nodiscard]] int GetSize() const;

        [[nodiscard]] float GetValue(int x, int y) const;

    private:
        static constexpr unsigned int texture_size = 256;
        float** absorption_texture;
    };
}
