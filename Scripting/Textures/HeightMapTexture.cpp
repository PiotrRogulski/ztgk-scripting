#include "HeightMapTexture.h"

namespace duck_app {
    HeightMapTexture::HeightMapTexture() {
        this->height_map_texture = new float*[this->texture_size];

        for (auto i = 0U; i < this->texture_size; i++) {
            this->height_map_texture[i] = new float[this->texture_size];
        }

        for (auto x = 0U; x < this->texture_size; x++) {
            for (auto y = 0U; y < this->texture_size; y++) {
                this->height_map_texture[x][y] = 0.0F;
            }
        }
    }

    //void HeightMapTexture::SetValue(int x, int y, float value)
    //{
    //    this->height_map_texture[x][y] = value;
    //}

    //float HeightMapTexture::GetValue(int x, int y)
    //{
    //    return this->height_map_texture[x][y];
    //}

    //int HeightMapTexture::GetSize()
    //{
    //    return this->texture_size;
    //}
}
