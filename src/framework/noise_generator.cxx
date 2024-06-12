#include "framework/noise_generator.hxx"

void generate_noise(std::vector<float>& buffer, const GenerateNoiseOptions& options) {
    const auto buffer_size = options.width * options.height;
    const auto centerX = options.width / 2.0f;
    const auto centerY = options.height / 2.0f;

    if (buffer.size() < buffer_size) {
        buffer.resize(buffer_size);
    }

    const float maxDistance = (std::max(options.width, options.height) / 2.0f) * options.radius_mult;

    for (i32 y = 0; y < options.height; ++y) {
        for (i32 x = 0; x < options.width; ++x) {
            float noise = 0.0f;
            float frequency = options.frequency;
            float amplitude = options.amplitude;
            float global_x = static_cast<float>(x + options.offset_x) / options.width;
            float global_y = static_cast<float>(y + options.offset_y) / options.height;

            for (i32 octave = 0; octave < options.octaves; ++octave) {
                noise += stb_perlin_noise3_seed(
                    global_x * frequency,
                    global_y * frequency,
                    0, // z * frequency,
                    0,
                    0,
                    0,
                    (unsigned char) octave
                ) * amplitude;

                frequency *= options.lacunarity;
                amplitude *= options.gain;
            }

            noise = std::clamp(noise, -1.0f, 1.0f);
            noise = (noise + 1.0f) / 2.0f;

            if (options.use_gradient) {
                float distance = std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
                float normalizedDistance = distance / maxDistance;
                float gradient = 1.0f - std::pow(normalizedDistance, options.gradient_falloff);
                noise *= gradient;
            }

            buffer[(y * options.width + x)] = noise;
        }
    }
}