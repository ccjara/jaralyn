#ifndef NOEDEN_WORLD_EVENT_HXX
#define NOEDEN_WORLD_EVENT_HXX

#include "framework/engine_event_type.hxx"

class WorldSpec;

struct WorldReadyEvent : public Event<EngineEventType::WorldReady> {
    WorldSpec* world_spec = nullptr;

    WorldReadyEvent(WorldSpec* world_spec) : world_spec(world_spec) {
        assert(world_spec);
    }
};

struct WorldUpdatedPreEvent : public Event<EngineEventType::WorldUpdatedPre> {
};

struct WorldUpdatedPostEvent : public Event<EngineEventType::WorldUpdatedPost> {
};

#endif
