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
            float noise = stb_perlin_fbm_noise3(
                (float) x * options.frequency,
                (float) y * options.frequency,
                0,
                options.lacunarity,
                options.gain,
                options.octaves
            ) * options.amplitude;

            noise = std::clamp(noise, -1.0f, 1.0f);
            noise = (noise + 1.0f) / 2.0f;

            if (options.use_gradient) {
                float distance = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
                float normalizedDistance = distance / maxDistance;
                float gradient = 1.0f - pow(normalizedDistance, options.gradient_falloff);
                noise *= gradient;
            }

            if (noise <= options.low_threshold) {
                noise = 0;
            }
            if (noise >= options.high_threshold) {
                noise = 1.0f;
            }

            buffer[(y * options.width + x)] = noise;
        }
    }
}