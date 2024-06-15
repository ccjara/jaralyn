#ifndef NOEDEN_TILE_HXX
#define NOEDEN_TILE_HXX

#include "../gfx/display_cell.hxx"

enum class TileType : unsigned char {
    Empty,
    Floor,
    Wall,
};

enum class MaterialType : unsigned char {
    None,
    Stone,
    Wood,
    Vegetation,
    Water,
};

enum class MaterialState : unsigned char {
    Void,
    Solid,
    Liquid,
};

enum TileFlags {
    /**
     * @brief No flags set
     */
    None,

    /**
     * @brief Whether the tile is a ramp
     */
    Ramp,

    /**
     * @brief Whether the tile is solid and cannot be passed through
     */
    Blocking,

    /**
     * @brief Whether this tile has been within FoV by the player before
     */
    Revealed,

    /**
     * @brief Whether the tile is currently within the FoV of any player controlled entity
     */
    FoV,
};

struct Tile {
    TileType type = TileType::Empty;

    MaterialType material = MaterialType::None;

    MaterialState state = MaterialState::Void;

    std::bitset<8> flags;
    
    DisplayCell display_info; // needs to be moved out later and determined based on tile type etc.
};

#endif
