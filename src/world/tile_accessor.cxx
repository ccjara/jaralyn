#include "world/tile_accessor.hxx"
#include "world/chunk_manager.hxx"
#include "world/chunk.hxx"

TileAccessor::TileAccessor(ChunkManager* chunk_manager) : chunk_manager_(chunk_manager) {
    assert(chunk_manager_);
}

Tile* TileAccessor::get_tile(const WorldPos& position) {
    if (position.x < 0 || position.y < 0 || position.z < 0) {
        return nullptr;
    }

    Chunk* chunk = chunk_manager_->get_chunk(position);
    if (!chunk) {
        return nullptr;
    }

    return &chunk->tiles[to_index(position)];
}

void TileAccessor::set_tile(const WorldPos& position, const Tile& tile) {
    Chunk* chunk = chunk_manager_->get_chunk(position);
    if (!chunk) {
        return;
    }
    chunk->tiles[to_index(position)] = tile;
}

size_t TileAccessor::to_index(const WorldPos& position) const {
    return
        position.y * Chunk::CHUNK_SIDE_LENGTH * Chunk::CHUNK_SIDE_LENGTH +
        position.z * Chunk::CHUNK_SIDE_LENGTH +
        position.x;
}
