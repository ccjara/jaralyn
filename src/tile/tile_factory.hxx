#ifndef JARALYN_TILE_FACTORY_HXX
#define JARALYN_TILE_FACTORY_HXX

#include "tile.hxx"

class TileFactory {
public:
    static Tile none();
    static Tile wall();
    static Tile floor();
    static Tile water();

    static Tile for_type(TileType type);
};

#endif
