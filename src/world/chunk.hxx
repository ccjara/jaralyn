#ifndef NOEDEN_CHUNK_HXX
#define NOEDEN_CHUNK_HXX

#include "tile/tile.hxx"

struct Chunk {
    static constexpr i32 CHUNK_SIDE_LENGTH = 64;
    static constexpr i32 CHUNK_DEPTH = 128;
    static constexpr i32 MAX_TILES = CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH * CHUNK_DEPTH;
    static constexpr i32 MAX_INDEX = MAX_TILES - 1;

    std::array<Tile, MAX_TILES> tiles;
};

#endif
