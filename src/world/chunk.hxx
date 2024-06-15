#ifndef NOEDEN_CHUNK_HXX
#define NOEDEN_CHUNK_HXX

#include "tile/tile.hxx"

struct Chunk {
    static constexpr i32 CHUNK_SIDE_LENGTH = 128;
    static constexpr i32 CHUNK_DEPTH = 16;
    static constexpr i32 MAX_TILES = CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH * CHUNK_DEPTH;
    static constexpr i32 MAX_INDEX = MAX_TILES - 1;

    /**
     * @brief Base height as determined by the world generator
     */
    i32 base_height = 0;

    /**
     * @brief Base humidity as determined by the world generator
     */
    float base_humidity = 0;

    std::array<i32, CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH> height_map = {0};

    std::array<Tile, MAX_TILES> tiles;

    inline Tile& tile(i32 x, i32 y, i32 z) {
        assert(x >= 0 && x < CHUNK_SIDE_LENGTH);
        assert(y >= 0 && y < CHUNK_DEPTH);
        assert(z >= 0 && z < CHUNK_SIDE_LENGTH);

        return tiles[x + z * CHUNK_SIDE_LENGTH + y * CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH];
    }
};

#endif
