#include "world/world_spec.hxx"

float WorldSpec::height_at(const ChunkPos& chunk_pos) const {
    const auto index = chunk_pos.z * chunks_x_ + chunk_pos.x;

    if (index < 0 || index >= height_map_.size()) {
        return 0.0f;
    }

    return height_map_[index];
}


float WorldSpec::humidity_at(const ChunkPos& chunk_pos) const {
    const auto index = chunk_pos.z * chunks_x_ + chunk_pos.x;

    if (index < 0 || index >= humidity_map_.size()) {
        return 0.0f;
    }

    return humidity_map_[index];
}

i32 WorldSpec::seed() const {
    return seed_;
}

i32 WorldSpec::version() const {
    return version_;
}

i32 WorldSpec::chunks_x() const {
    return chunks_x_;
}

i32 WorldSpec::chunks_z() const {
    return chunks_z_;
}

const GenerateNoiseOptions& WorldSpec::height_map_options() const {
    return height_map_options_;
}

i32 WorldSpec::max_vegetation() const {
    return max_vegetation_;
}

i32 WorldSpec::max_shoreline() const {
    return max_shoreline_;
}

i32 WorldSpec::max_water() const {
    return max_water_;
}

