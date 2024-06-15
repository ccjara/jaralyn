#include "world/world_spec_creator.hxx"
#include "world/world_spec.hxx"
#include "framework/noise_generator.hxx"

std::unique_ptr<WorldSpec> WorldSpecCreator::create_world_spec(const CreateWorldSpecOptions& options) {
    auto spec = std::make_unique<WorldSpec>();
    spec->version_ = 1;
    spec->seed_ = options.seed;
    spec->chunks_x_ = options.chunks_x;
    spec->chunks_z_ = options.chunks_z;
    spec->max_vegetation_ = options.max_vegetation;
    spec->max_shoreline_ = options.max_shoreline;
    spec->max_water_ = options.max_water;

    generate_height_map(*spec);
    generate_humidity_map(*spec);

    return spec;
}

void WorldSpecCreator::generate_height_map(WorldSpec& spec) {
    spec.height_map_.resize(spec.chunks_x_ * spec.chunks_z_);
    GenerateNoiseOptions options {
        .frequency = 1.5f,
        .amplitude = 1.0f,
        .width = spec.chunks_x_,
        .height = spec.chunks_z_,
        .seed = spec.seed_,
        .z = 0,
        .lacunarity = 2.0f,
        .gain = 0.5f,
        .use_gradient = false,
    };

    spec.height_map_options_ = options;
    spec.height_map_ = generate_noise(options);
}

void WorldSpecCreator::generate_humidity_map(WorldSpec& spec) {
    spec.humidity_map_.resize(spec.chunks_x_ * spec.chunks_z_);
    GenerateNoiseOptions options {
        .frequency = 0.02f,
        .width = spec.chunks_x_,
        .height = spec.chunks_z_,
        .seed = spec.seed_,
        .lacunarity = 2.0f,
        .gain = 0.5f,
        .use_gradient = false,
    };

    spec.height_map_options_ = options;
    spec.height_map_ = generate_noise(options);
}
