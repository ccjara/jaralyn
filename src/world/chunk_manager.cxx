#include "entity/entity.hxx"
#include "world/chunk_manager.hxx"
#include "world/chunk_generator.hxx"
#include "world/chunk.hxx"
#include "world/world_spec.hxx"

ChunkManager::ChunkManager(ChunkGenerator* chunk_generator, Events* events) :
    chunk_generator_(chunk_generator),
    events_(events) {
    assert(events_);
    assert(chunk_generator_);

    on_world_ready_sub_ = events_->engine->on<WorldReadyEvent>(this, &ChunkManager::on_world_ready);
    on_entity_created_sub_ = events_->engine->on<EntityCreatedEvent>(this, &ChunkManager::on_entity_created);
    on_player_moved_sub_ = events_->engine->on<PlayerMovedEvent>(this, &ChunkManager::on_player_moved);
}

constexpr std::size_t ChunkManager::ChunkPosHasher::operator()(const ChunkPos& pos) const {
    return pos.x * (Chunk::CHUNK_SIDE_LENGTH + 1) + pos.z;
};

Chunk* ChunkManager::get_chunk(const WorldPos& position) {
    auto it = chunks_.find(to_chunk_pos(position));
    if (it == chunks_.end()) {
        return nullptr;
    }
    return it->second.get();
}

Chunk* ChunkManager::create_chunk(const ChunkPos& position) {
    assert(world_spec_ != nullptr);

    if (chunks_.contains(position)) {
        return nullptr;
    }

    GenerateChunkOptions options = {
        .world_spec = *world_spec_,
        .position = position,
    };

    std::unique_ptr<Chunk> chunk = chunk_generator_->generate_chunk(options);
    Chunk* raw_ptr = chunk.get();
    chunks_[position] = std::move(chunk);

    Log::debug("Chunk created at ({}, {})", position.x, position.z);

    return raw_ptr;
}

ChunkPos ChunkManager::to_chunk_pos(const WorldPos& position) const {
    return {
        position.x / Chunk::CHUNK_SIDE_LENGTH,
        position.z / Chunk::CHUNK_SIDE_LENGTH
    };
}

EventResult ChunkManager::on_world_ready(const WorldReadyEvent& e) {
    world_spec_ = e.world_spec;
    Log::debug("World spec updated in ChunkManager");

    return EventResult::Continue;
}

EventResult ChunkManager::on_entity_created(const EntityCreatedEvent& e) {
    Log::debug("Ensuring chunk for entity {} at {}", e.entity->name, e.entity->position.to_string());
    create_chunk(to_chunk_pos(e.entity->position));
    return EventResult::Continue;
}

EventResult ChunkManager::on_player_moved(const PlayerMovedEvent& e) {
    // if too close to the edge of the current chunk, load the next chunk
    constexpr i32 CHUNK_LOAD_DISTANCE = 8;

    const auto current_chunk_pos = to_chunk_pos(e.position);
    const auto local = WorldPos(
        e.position.x % Chunk::CHUNK_SIDE_LENGTH,
        e.position.y,
        e.position.z % Chunk::CHUNK_SIDE_LENGTH
    );

    if (local.x < CHUNK_LOAD_DISTANCE && current_chunk_pos.x > 0) {
        create_chunk(ChunkPos(current_chunk_pos.x - 1, current_chunk_pos.z));
    } else if (local.x > Chunk::CHUNK_SIDE_LENGTH - CHUNK_LOAD_DISTANCE && current_chunk_pos.x < world_spec_->chunks_x() - 1) {
        create_chunk(ChunkPos(current_chunk_pos.x + 1, current_chunk_pos.z));
    }

    if (local.z < CHUNK_LOAD_DISTANCE && current_chunk_pos.z > 0) {
        create_chunk(ChunkPos(current_chunk_pos.x, current_chunk_pos.z - 1));
    } else if (local.z > Chunk::CHUNK_SIDE_LENGTH - CHUNK_LOAD_DISTANCE && current_chunk_pos.z < world_spec_->chunks_z() - 1) {
        create_chunk(ChunkPos(current_chunk_pos.x, current_chunk_pos.z + 1));
    }

    create_chunk(to_chunk_pos(e.position));
    return EventResult::Continue;
}
