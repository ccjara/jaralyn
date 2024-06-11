#ifndef NOEDEN_CHUNK_HXX
#define NOEDEN_CHUNK_HXX

#include "tile/tile.hxx"

struct Chunk {
    static constexpr i32 CHUNK_SIDE_LENGTH = 64;
    static constexpr i32 CHUNK_DEPTH = 128;

    std::array<Tile, CHUNK_SIDE_LENGTH * CHUNK_SIDE_LENGTH * CHUNK_DEPTH> tiles;
};

#endif
