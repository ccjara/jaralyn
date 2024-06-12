#include "entity/entity.hxx"

Entity::Entity() : id(next_id_++) {
}

bool Entity::add_component(std::unique_ptr<Component>&& component) {
    if (!component) {
        Log::error("Cannot add null component to entity {}", id);
        return false;
    }
    const auto component_type = component->type();

    if (has_component(component_type)) {
        Log::error("Cannot add component {} to entity {}: already exists", (u32) component_type, id);
        return false; // as no move of component took place the resource will be freed
    }

    component->set_owner(this);
    components_by_type_[component_type] = component.get();
    components_.push_back(std::move(component));

    return true;
}

bool Entity::has_component(ComponentType type) const {
    return components_by_type_.find(type) != components_by_type_.end();
}

void Entity::update(float energy_received) {
    this->energy += energy_received;

    for (auto& component : components_) {
        component->update();
    }
}

void Entity::on_after_actions() {
    energy_reserved = 0;

    for (auto& component : components_) {
        component->on_after_actions();
    }
}

void Entity::on_player_attached() {
    player_attached_ = true;
    for (auto& component : components_) {
        component->on_player_attached();
    }
}

void Entity::on_player_detached() {
    player_attached_ = false;
    for (auto& component : components_) {
        component->on_player_detached();
    }
}
