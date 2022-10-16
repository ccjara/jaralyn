#include "entity_factory.hxx"

std::unique_ptr<Entity> EntityFactory::create(const Archetype& archetype) {
    auto entity = std::make_unique<Entity>();

    entity->copy_components(archetype.components.cbegin(), archetype.components.cend());

    return entity;
}
