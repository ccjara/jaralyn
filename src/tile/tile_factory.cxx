#include "tile_factory.hxx"

Tile TileFactory::none() {
    Tile t;
    t.display_info.glyph = ' ';
    t.display_info.color = Color::black();
    return t;
}

Tile TileFactory::wall() {
    Tile t;
    t.type = TileType::Wall;
    t.state = MaterialState::Solid;
    t.material = MaterialType::Stone;
    t.flags.set(TileFlags::Blocking);

    t.display_info.glyph = 745;
    t.display_info.color = Color::mono(64);
    return t;
}

Tile TileFactory::grass() {
    Tile t;
    t.type = TileType::Floor;
    t.state = MaterialState::Solid;
    t.material = MaterialType::Vegetation;

    switch (std::rand() % 5) {
    case 0:
        t.display_info.glyph = 39; // '
        break;
    case 1:
        t.display_info.glyph = 44; // ,
        break;
    case 2:
        t.display_info.glyph = 46; // .
        break;
    case 3:
        t.display_info.glyph = 96; // `
        break;
    case 4:
        t.display_info.glyph = '"'; // "
        break;
    }

    t.display_info.color = Color::green();
    return t;
}

Tile TileFactory::water() {
    Tile t;
    t.type = TileType::Floor;
    t.state = MaterialState::Liquid;
    t.material = MaterialType::Water;

    t.display_info.glyph = 691; // ≈
    t.display_info.color = Color::blue();
    return t;
}
