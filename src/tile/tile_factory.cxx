#include "tile_factory.hxx"

Tile TileFactory::none() {
    Tile none;
    none.type = TileType::None;
    none.display_info.glyph = ' ';
    none.display_info.color = Color::white();
    none.solid = false;
    return none;
}

Tile TileFactory::wall() {
    Tile wall;
    wall.type = TileType::Wall;
    wall.display_info.glyph = '#';
    wall.display_info.color = Color::white();
    wall.solid = true;
    return wall;
}

Tile TileFactory::floor() {
    Tile floor;
    floor.type = TileType::Floor;
    floor.display_info.glyph = '.';
    floor.display_info.color = Color::white();
    floor.solid = false;
    return floor;
}

Tile TileFactory::water() {
    Tile floor;
    floor.type = TileType::Water;
    floor.display_info.glyph = 691; // ≈
    floor.display_info.color = Color::blue();
    floor.solid = false;
    return floor;
}

Tile TileFactory::for_type(TileType type) {
    switch (type) {
        case TileType::None:
        default:
            return none();
        case TileType::Water:
            return water();
        case TileType::Floor:
            return TileFactory::floor();
        case TileType::Wall:
            return wall();
    }
}