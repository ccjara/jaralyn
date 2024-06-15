#include "framework/noise_generator.hxx"

std::vector<float> generate_noise(const GenerateNoiseOptions& options) {
    std::vector<float> buffer(options.width * options.height);
    if (buffer.empty()) {
        return buffer;
    }
    const auto center_x = 0.5f * options.width;
    const auto center_y = 0.5f * options.height;

    float max_distance = static_cast<float>(std::max(options.width, options.height)) * 0.5f * options.radius_mult;

    if (max_distance <= 0) {
        max_distance = 0.01f;
    }

    // track min / max for normalization
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::lowest();

    for (i32 y = 0; y < options.height; ++y) {
        for (i32 x = 0; x < options.width; ++x) {
            float noise = 0.0f;
            float frequency = options.frequency;
            float amplitude = options.amplitude;
            float global_x = static_cast<float>(x + options.offset_x) / static_cast<float>(options.width);
            float global_y = static_cast<float>(y + options.offset_y) / static_cast<float>(options.height);

            // apply fbm-like noise
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

            if (noise < min) {
                min = noise;
            }
            if (noise > max) {
                max = noise;
            }

            buffer[(y * options.width + x)] = noise;
        }
    }


    if (options.use_gradient) {
        // normalize, then apply gradient
        for (i32 y = 0; y < options.height; ++y) {
            for (i32 x = 0; x < options.width; ++x) {
                float noise = buffer[(y * options.width + x)];
                float min_max_delta = max - min;

                if (min_max_delta != 0) {
                    noise = (noise - min) / min_max_delta;
                }

                float dx = static_cast<float>(x) - center_x;
                float dy = static_cast<float>(y) - center_y;
                float distance = std::sqrt(dx * dx + dy * dy);

                noise *= 1.0f - std::min(distance / max_distance, 1.0f);

                noise = std::pow(noise, options.gradient_falloff);

                //if (noise < 0.0f) {
                //    noise = 0.0f;
                //} else if (noise > 1.0f) {
                //    noise = 1.0f;
                //}

                buffer[(y * options.width + x)] = noise;
            }
        }
    } else {
        // just normalize
        if (max - min != 0) {
            for(auto& value : buffer) {
                value = (value - min) / (max - min);

                if (value < 0.0f) {
                    value = 0.0f;
                } else if (value > 1.0f) {
                    value = 1.0f;
                }
            }
        }
    }

    return buffer;
}
