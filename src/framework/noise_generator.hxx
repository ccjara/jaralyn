#ifndef NOEDEN_NOISE_GENERATOR_HXX
#define NOEDEN_NOISE_GENERATOR_HXX

struct GenerateNoiseOptions {
    float frequency = 1;
    float amplitude = 1;
    i32 width = 256;
    i32 height = 256;
    i32 seed = 0;
    i32 octaves = 4;
    float z = 0;
    float lacunarity = 3.0f;
    float gain = 0.517f;
    float low_threshold = 0.517f;
    float high_threshold = low_threshold;
    bool use_gradient = false;
    float radius_mult = 1.0f;
    float gradient_falloff = 5.0f;
};

void generate_noise(std::vector<float>& buffer, const GenerateNoiseOptions& options);

#endif
