#include "world/world_spec_creator.hxx"
#include "world/world_spec.hxx"
#include "framework/noise_generator.hxx"

std::unique_ptr<WorldSpec> WorldSpecCreator::create_world_spec(const CreateWorldSpecOptions& options) {
    auto spec = std::make_unique<WorldSpec>();
    spec->version_ = 1;
    spec->seed_ = options.seed;
    spec->chunks_x_ = options.chunks_x;
    spec->chunks_z_ = options.chunks_z;

    // generate height map
    spec->height_map_.resize(spec->chunks_x_ * spec->chunks_z_);
    GenerateNoiseOptions height_map_options {
        .frequency = 0.015f,
        .amplitude = 1.0f,
        .width = spec->chunks_x_,
        .height = spec->chunks_z_,
        .seed = spec->seed_,
        .z = 0,
        .lacunarity = 3.0f,
        .gain = 0.517f,
        .low_threshold = 0.517f,
        .high_threshold = 0.517f,
        .use_gradient = true,
        .radius_mult = 1.0f,
        .gradient_falloff = 5.0f
    };

    generate_noise(spec->height_map_, height_map_options);

    return spec;
}
