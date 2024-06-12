#ifndef NOEDEN_CHUNK_MANAGER_HXX
#define NOEDEN_CHUNK_MANAGER_HXX

#include "entity/entity_event.hxx"
#include "world/world_event.hxx"

class Chunk;
class ChunkGenerator;

class ChunkManager {
public:
    explicit ChunkManager(ChunkGenerator* chunk_generator, Events* events);

    /**
     * @brief Returns the chunk at the given world position or nullptr if not existing
     */
    Chunk* get_chunk(const WorldPos& position);

    /**
     * @brief Generates a new chunk at the given position if it does not exist yet.
     *
     * @return A pointer to the generated chunk or nullptr if already existing or on error.
     */
     Chunk* create_chunk(const ChunkPos& position);
private:
    struct ChunkPosHasher {
        constexpr std::size_t operator()(const ChunkPos& pos) const;
    };

    std::unordered_map<ChunkPos, std::unique_ptr<Chunk>, ChunkPosHasher> chunks_;

    /**
     * @brief Converts the given world position to the position of the chunk that would contain the world position.
     */
    ChunkPos to_chunk_pos(const WorldPos& position) const;

    Events* events_ = nullptr;
    ChunkGenerator* chunk_generator_ = nullptr;
    WorldSpec* world_spec_ = nullptr;

    Subscription<WorldReadyEvent> on_world_ready_sub_;
    Subscription<EntityCreatedEvent> on_entity_created_sub_;
    Subscription<PlayerMovedEvent> on_player_moved_sub_;
    EventResult on_world_ready(const WorldReadyEvent& e);
    EventResult on_entity_created(const EntityCreatedEvent& e);
    EventResult on_player_moved(const PlayerMovedEvent& e);
};

#endif
