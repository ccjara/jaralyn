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

    const auto index = to_local_index(position);
    if (index > Chunk::MAX_INDEX) {
        return nullptr;
    }

    return &chunk->tiles[index];
}

void TileAccessor::set_tile(const WorldPos& position, const Tile& tile) {
    Chunk* chunk = chunk_manager_->get_chunk(position);
    if (!chunk) {
        return;
    }
    chunk->tiles[to_local_index(position)] = tile;
}

size_t TileAccessor::to_local_index(WorldPos position) const {
    position.x %= Chunk::CHUNK_SIDE_LENGTH;
    // position.y %= Chunk::CHUNK_DEPTH; // not needed with 2D chunks
    position.z %= Chunk::CHUNK_SIDE_LENGTH;

    return
        position.y * Chunk::CHUNK_SIDE_LENGTH * Chunk::CHUNK_SIDE_LENGTH +
        position.z * Chunk::CHUNK_SIDE_LENGTH +
        position.x;
}
