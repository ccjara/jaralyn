#ifndef NOEDEN_NOISE_GENERATOR_HXX
#define NOEDEN_NOISE_GENERATOR_HXX

struct GenerateNoiseOptions {
    float frequency = 3.5f;
    float amplitude = 1.5f;
    i32 width = 256;
    i32 height = 256;
    i32 offset_x = 0;
    i32 offset_y = 0;
    i32 seed = 0;
    i32 octaves = 4;
    float z = 0;
    float lacunarity = 3.0f;
    float gain = 0.5f;
    bool use_gradient = false;
    float radius_mult = 1.0f;
    float gradient_falloff = 1.0f;
};

std::vector<float> generate_noise(const GenerateNoiseOptions& options);

#endif
