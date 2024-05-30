#include "entity/entity_factory.hxx"
#include "entity/entity.hxx"
#include "entity/archetype.hxx"

std::unique_ptr<Entity> EntityFactory::create(const Archetype& archetype) {
    auto entity = std::make_unique<Entity>();

    // copy common properties from archetype into entity
    entity->speed = archetype.speed;
    entity->name = archetype.name;

    for (const auto& archetype_component : archetype.components) {
        entity->add_component(archetype_component->clone());
    }

    return entity;
}
