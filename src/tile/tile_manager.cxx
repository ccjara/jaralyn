#include "tile_manager.hxx"
#include "tile_factory.hxx"

TileManager::TileManager() {
    tiles_.resize({ 120, 40 });
    for (auto& t : tiles_.cells()) {
        t = TileFactory::floor();
    }
}

Tile* TileManager::tile(Vec2<i32> position) {
    return tiles_.at(position);
}


const Tile* TileManager::tile(Vec2<i32> position) const {
    return tiles_.at(position);
}

Grid<Tile> const& TileManager::tiles() const {
    return tiles_;
}

Grid<Tile>& TileManager::tiles() {
    return tiles_;
}

Tile& TileManager::create_tile(TileType type, Vec2<i32> position) {
    auto t = TileFactory::for_type(type);
    tiles_.put(t, position);
    return *tiles_.at(position);
}

