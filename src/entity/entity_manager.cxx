#include "entity/entity_manager.hxx"
#include "entity/entity.hxx"
#include "entity/entity_factory.hxx"
#include "entity/archetype.hxx"

Entity* EntityManager::entity(Id id) {
    auto it = index_by_id_.find(id);
    if (it == index_by_id_.end()) {
        return nullptr;
    }
    if (it->second >= entities_.size()) {
        return nullptr;
    }
    return entities_[it->second].get();
}

const Entity* EntityManager::entity(Id id) const {
    return entity(id);
}

EntityManager::EntityContainer& EntityManager::entities() {
    return entities_;
}

const EntityManager::EntityContainer& EntityManager::entities() const {
    return entities_;
}

Entity& EntityManager::create_entity(const Archetype& archetype) {
    entities_.push_back(EntityFactory::create(archetype));
    auto& entity = entities_.back();

    index_by_id_[entity->id] = entities_.size() - 1;
    return *entity.get();
}

Entity* EntityManager::player() {
    return controlled_entity_;
}

const Entity* EntityManager::player() const {
    return controlled_entity_;
}

ControlEntityResult EntityManager::set_controlled_entity(Id id) {
    if (id == null_id) {
        return set_controlled_entity(nullptr);
    }

    Entity* next_player = entity(id);
    if (next_player == nullptr) {
        return ControlEntityResult::EntityNotFound;
    }
    return set_controlled_entity(next_player);
}

ControlEntityResult EntityManager::set_controlled_entity(Entity* entity) {
    Entity* current_player = player();
    if (current_player) {
        current_player->on_player_detached();
    }

    if (entity != nullptr) {
        entity->energy = 0;
        entity->on_player_attached();
    }
    controlled_entity_ = entity;

    return ControlEntityResult::Success;
}
