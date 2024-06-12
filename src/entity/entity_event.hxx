#ifndef NOEDEN_ENTITY_EVENT_HXX
#define NOEDEN_ENTITY_EVENT_HXX

#include "framework/engine_event_type.hxx"

class Entity;

struct PlayerMovedEvent : public Event<EngineEventType::PlayerMoved> {
    WorldPos position;

    explicit PlayerMovedEvent(const WorldPos& position) : position(position) {}
};

struct EntityCreatedEvent : public Event<EngineEventType::EntityCreated> {
    Entity* entity = nullptr;

    explicit EntityCreatedEvent(Entity* entity) : entity(entity) {
        assert(entity);
    }
};

#endif
