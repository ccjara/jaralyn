#ifndef JARALYN_TILE_HXX
#define JARALYN_TILE_HXX

#include "../gfx/display_cell.hxx"

enum class TileType {
    None,
    Floor,
    Wall,
    Water,
};

// TODO: use flags instead of separate bools

struct Tile {
    TileType type { TileType::Wall };
    
    bool solid { true };

    /**
     * @brief Whether the tile is currently within the FoV of any player controlled unit
     */
    bool fov { false };

    /**
     * @brief Whether this tile has been within FoV by the player before
     */
    bool revealed { true };

    DisplayCell display_info;
};

#endif
