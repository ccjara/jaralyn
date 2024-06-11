#include "world/chunk_generator.hxx"
#include "world/chunk.hxx"
#include "world/world_spec.hxx"

std::unique_ptr<Chunk> ChunkGenerator::generate_chunk(const GenerateChunkOptions& options) {
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();

    for (i32 x = 0; x < Chunk::CHUNK_SIDE_LENGTH; x++) {
        for (i32 z = 0; z < Chunk::CHUNK_SIDE_LENGTH; z++) {
            for (i32 y = 0; y < Chunk::CHUNK_DEPTH; y++) {
                const auto index = x + z * Chunk::CHUNK_SIDE_LENGTH + y * Chunk::CHUNK_SIDE_LENGTH * Chunk::CHUNK_SIDE_LENGTH;
                Tile& tile = chunk->tiles[index];

                tile.display_info.glyph = 'X';

                if (x == 0 || z == 0) {
                    tile.display_info.color = Color::red();
                } else {
                    tile.display_info.color = Color::white();
                }

                tile.material = MaterialType::Stone;
                tile.state = MaterialState::Solid;
                tile.type = TileType::Ground;
                tile.flags.set(TileFlags::FoV, true);
                tile.flags.set(TileFlags::Revealed, true);
            }
        }
    }
    return chunk;
}
