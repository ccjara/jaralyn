#include "component.hxx"

Component::Component(ComponentType type) : type_(type) {
}

ComponentType Component::type() const {
    return type_;
}

u64 Component::entity_id() const {
    return entity_id_;
}

void Component::set_entity_id(Id entity_id) {
    entity_id_ = entity_id;
}

void Component::update(u64 dt) {
}
