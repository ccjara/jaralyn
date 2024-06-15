#include "noise_texture.hxx"

NoiseTexture::NoiseTexture() {
    glGenTextures(1, &texture_);

    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void NoiseTexture::set_noise(const std::vector<float>& noise, i32 width, i32 height) {
    width_ = width;
    height_ = height;

    if (noise.size() != width_ * height_) {
        Log::warn("NoiseTexture::set_noise: noise buffer size does not match width * height");
        return;
    }

    fill_texture(noise);
}

NoiseTexture::~NoiseTexture() {
    if (texture_) {
        glDeleteTextures(1, &texture_);
    }
}

void NoiseTexture::draw() const {
    if (!texture_ || !width_ || !height_) {
        return;
    }
    ImGui::Image(
        reinterpret_cast<void*>(static_cast<intptr_t>(texture_)),
        ImVec2(static_cast<float>(width_), static_cast<float>(height_))
    );
}

void NoiseTexture::fill_texture(const std::vector<float>& noise) const {
    std::vector<float> buffer(width_ * height_ * 3, 0.0f);

    for (i32 y = 0; y < height_; ++y) {
        for (i32 x = 0; x < width_; ++x) {
            const size_t noise_index = y * width_ + x;
            const float value = noise[noise_index];
            const size_t offset_r = noise_index * 3;
            buffer[offset_r] = value;
            buffer[offset_r + 1] = value;
            buffer[offset_r + 2] = value;
        }
    }

    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width_,
        height_,
        0,
        GL_RGB,
        GL_FLOAT,
        buffer.data()
    );
}

