#include "world/chunk_generator.hxx"
#include "world/chunk.hxx"
#include "world/world_spec.hxx"
#include "framework/noise_generator.hxx"

std::unique_ptr<Chunk> ChunkGenerator::generate_chunk(const GenerateChunkOptions& options) {
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();

    GenerateNoiseOptions height_map_options = options.world_spec.height_map_options();

    height_map_options.width = Chunk::CHUNK_SIDE_LENGTH;
    height_map_options.height = Chunk::CHUNK_SIDE_LENGTH;

    height_map_options.use_gradient = false;
    height_map_options.offset_x = options.position.x * Chunk::CHUNK_SIDE_LENGTH;
    height_map_options.offset_y = options.position.z * Chunk::CHUNK_SIDE_LENGTH;

    std::vector<float> height_map;
    generate_noise(height_map, height_map_options);

    const auto chunk_max_height = options.world_spec.height_at(options.position);

    const i32 max_water = options.world_spec.max_water();
    const i32 max_shoreline = options.world_spec.max_shoreline();
    const i32 max_vegetation = options.world_spec.max_vegetation();

    for (i32 x = 0; x < Chunk::CHUNK_SIDE_LENGTH; ++x) {
        for (i32 z = 0; z < Chunk::CHUNK_SIDE_LENGTH; ++z) {
            const auto height_value = static_cast<i32>(height_map[x + z * Chunk::CHUNK_SIDE_LENGTH] * Chunk::CHUNK_DEPTH);
            for (i32 y = 0; y < height_value; ++y) {
                const auto index = x + z * Chunk::CHUNK_SIDE_LENGTH + y * Chunk::CHUNK_SIDE_LENGTH * Chunk::CHUNK_SIDE_LENGTH;
                assert(index <= Chunk::MAX_INDEX);
                Tile& tile = chunk->tiles[index];

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

                tile.type = TileType::Ground;
                tile.flags.set(TileFlags::FoV, true);
                tile.flags.set(TileFlags::Revealed, true);
            }
        }
    }
    return chunk;
}
