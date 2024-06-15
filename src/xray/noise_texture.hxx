#ifndef NOEDEN_NOISE_TEXTURE_HXX
#define NOEDEN_NOISE_TEXTURE_HXX

class NoiseTexture {
public:
    NoiseTexture();

    virtual void draw() const;

    void set_noise(const std::vector<float>& noise, i32 width, i32 height);

    virtual ~NoiseTexture();
private:
    virtual void fill_texture(const std::vector<float>& noise) const;

    i32 width_ = 0;
    i32 height_ = 0;
    GLuint texture_ = 0;
};

#endif
