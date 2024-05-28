#ifndef JARALYN_TILE_BUILDER_HXX
#define JARALYN_TILE_BUILDER_HXX

#include "tile.hxx"

class TileBuilder {
public:
    static Tile none();
    static Tile wall();
    static Tile floor();
    static Tile water();

    static Tile for_type(TileType type);
};

#endif
