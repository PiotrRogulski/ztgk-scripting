#pragma once

namespace duck_app {
    class HeightMapTexture {
    public:
        HeightMapTexture();

        void SetValue(const int x, const int y, const float value) const {
            this->height_map_texture[x][y] = value;
        }

        [[nodiscard]] int GetSize() const {
            return this->texture_size;
        }

        [[nodiscard]] float GetValue(const int x, const int y) const {
            return this->height_map_texture[x][y];
        }

    private:
        static constexpr unsigned int texture_size = 256;
        float** height_map_texture;
    };
}
