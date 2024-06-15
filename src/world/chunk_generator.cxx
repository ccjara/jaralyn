#include "world/chunk_generator.hxx"
#include "world/chunk.hxx"
#include "world/world_spec.hxx"
#include "framework/noise_generator.hxx"

std::unique_ptr<Chunk> ChunkGenerator::generate_chunk(const GenerateChunkOptions& options) {
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();

    chunk->base_height = static_cast<i32>(options.world_spec.height_at(options.position) * Chunk::CHUNK_DEPTH);
    Log::debug("Chunk at ({}, {}) has base height of {}", options.position.x, options.position.z, chunk->base_height);

    GenerateNoiseOptions height_map_options = options.world_spec.height_map_options();
    height_map_options.width = Chunk::CHUNK_SIDE_LENGTH;
    height_map_options.height = Chunk::CHUNK_SIDE_LENGTH;
    height_map_options.frequency = 3;
    height_map_options.offset_x = options.position.x * Chunk::CHUNK_SIDE_LENGTH;
    height_map_options.offset_y = options.position.z * Chunk::CHUNK_SIDE_LENGTH;
    height_map_options.use_gradient = false;

    std::vector<float> height_noise = generate_noise(height_map_options);

    for (i32 i = 0; i < height_noise.size(); i++) {
        // convert to y
        i32 y = static_cast<i32>(chunk->base_height * (Chunk::CHUNK_DEPTH - 1));

        // value = chunk_base_height + value * 0.002f;
        y += -1.0f + height_noise[i] * (1.0f - -1.0f); // normalize to -1.0f to 1.0f (from 0.0f to 1.0f

        chunk->height_map[i] = std::clamp(y, 0, Chunk::CHUNK_DEPTH - 1);
    }

    const i32 max_water = options.world_spec.max_water();
    const i32 max_shoreline = options.world_spec.max_shoreline();
    const i32 max_vegetation = options.world_spec.max_vegetation();

    for (i32 x = 0; x < Chunk::CHUNK_SIDE_LENGTH; ++x) {
        for (i32 z = 0; z < Chunk::CHUNK_SIDE_LENGTH; ++z) {
            const auto height_value = chunk->height_map[x + z * Chunk::CHUNK_SIDE_LENGTH];

            for (i32 y = 0; y < height_value; ++y) {
                Tile& tile = chunk->tile(x, y, z);

                tile.type = TileType::Wall;
                tile.flags.set(TileFlags::FoV, y == height_value);
                tile.flags.set(TileFlags::Revealed, y == height_value);

                if (height_value <= max_water) {
                    tile.display_info.glyph = 691;
                    tile.display_info.color = Color::blue();
                    tile.material = MaterialType::Water;
                    tile.state = MaterialState::Liquid;
                } else if (height_value <= max_shoreline) {
                    tile.display_info.glyph = 748;
                    tile.display_info.color = Color::yellow();
                    tile.material = MaterialType::Stone;
                    tile.state = MaterialState::Solid;
                } else if (height_value <= max_vegetation) {
                    switch (std::rand() % 5) {
                        case 0:
                            tile.display_info.glyph = 39; // '
                            break;
                        case 1:
                            tile.display_info.glyph = 44; // ,
                            break;
                        case 2:
                            tile.display_info.glyph = 46; // .
                            break;
                        case 3:
                            tile.display_info.glyph = 96; // `
                            break;
                        case 4:
                            tile.display_info.glyph = '"'; // "
                            break;
                    }
                    tile.display_info.color = Color::green();
                    tile.material = MaterialType::Vegetation;
                    tile.state = MaterialState::Solid;
                } else {
                    tile.display_info.glyph = 679;
                    tile.display_info.color = Color::mono(64);
                    tile.material = MaterialType::Stone;
                    tile.state = MaterialState::Solid;
                }
            }

            // flooring after reaching chunk height
            if (height_value > 0 && height_value < Chunk::CHUNK_DEPTH) {
                Tile* tile_below = &chunk->tile(x, height_value - 1, z);
                Tile* tile = &chunk->tile(x, height_value, z);

                *tile = *tile_below;
                tile->type = TileType::Floor;
                tile->flags.set(TileFlags::FoV, true);
                tile->flags.set(TileFlags::Revealed, true);
            }
        }
    }

    // place ramps
    for (i32 z = 1; z < Chunk::CHUNK_SIDE_LENGTH; ++z) {
        for (i32 x = 1; x < Chunk::CHUNK_SIDE_LENGTH; ++x) {
            const i32 height_value = chunk->height_map[x + z * Chunk::CHUNK_SIDE_LENGTH];
            const i32 height_left = chunk->height_map[(x - 1) + z * Chunk::CHUNK_SIDE_LENGTH];
            const i32 height_back = chunk->height_map[x + (z - 1) * Chunk::CHUNK_SIDE_LENGTH];

            const auto delta_left = height_value - height_left;
            const auto delta_back = height_value - height_back;

            if (delta_left == 1) { // ‗= place ramp at ‗ (we are at =)
                Tile& rampUp = chunk->tile(x - 1, height_left, z);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x - 1, height_value, z);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            } else if (delta_left == -1) { // =‗ place ramp at ‗ (we are at ‗)
                Tile& rampUp = chunk->tile(x, height_value, z);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x, height_left, z);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            }

            if (delta_back == 1) { // place northern ramp
                Tile& rampUp = chunk->tile(x, height_back, z - 1);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x, height_value, z - 1);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            } else if (delta_back == -1) { // place southern ramp
                Tile& rampUp = chunk->tile(x, height_value, z);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x, height_back, z);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            }
        }
    }

    return chunk;
}
