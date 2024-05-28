#include "world.hxx"
#include "../entity/entity.hxx"
#include "../scene/scene.hxx"

Entity* World::get_entity_by_id(Id id) {
    return Scene::get_entity_by_id(id);
}
