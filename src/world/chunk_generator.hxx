#ifndef NOEDEN_CHUNK_GENERATOR_HXX
#define NOEDEN_CHUNK_GENERATOR_HXX

struct Chunk;
class WorldSpec;

struct GenerateChunkOptions {
    /**
     * @brief World spec to use for generation
     */
    const WorldSpec& world_spec;

    /**
     * @brief Where this chunk is going to be created. Important for noise generation
     */
    ChunkPos position;
};

class ChunkGenerator {
public:
    /**
     * @brief Generates a new chunk with the given options
     */
    [[nodiscard]] std::unique_ptr<Chunk> generate_chunk(const GenerateChunkOptions& options);
};

#endif
