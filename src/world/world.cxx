#include "world.hxx"
#include "../entity/entity.hxx"
#include "../entity/archetype.hxx"
#include "../scene/scene.hxx"

Entity* World::entity(Id id) {
    return Scene::entity(id);
}

World::EntityContainer& World::entities() {
    return Scene::entities();
}

const World::EntityContainer& World::entities() const {
    return Scene::read_entities();
}

Entity& World::create_entity(const Archetype& archetype) {
    return Scene::create_entity(archetype);
}

Entity* World::player() {
    return Scene::player();
}

const Entity* World::player() const {
    return Scene::player();
}

Tile* World::tile(Vec2<i32> position) const {
    return Scene::tiles().at(position);
}

Grid<Tile> const& World::tiles() const {
    return Scene::read_tiles();
}

Grid<Tile>& World::tiles() {
    return Scene::tiles();
}
