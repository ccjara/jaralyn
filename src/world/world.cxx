#include "world/world.hxx"
#include "action/action.hxx"
#include "action/action_processor.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"
#include "world/world_event.hxx"
#include "world/camera.hxx"

World::World(
    IEntityReader* entity_reader,
    IActionProcessor* action_processor,
    Events* events
) : entity_reader_(entity_reader),
    events_(events),
    action_processor_(action_processor),
    camera_(std::make_unique<Camera>()),
    camera_controller_(std::make_unique<CameraController>(entity_reader, events)) {
    assert(entity_reader_);
    assert(events_);
    assert(action_processor_);

    player_action_committed_sub_ = events_->engine->on<PlayerActionCommitted>(this, &World::on_player_action_committed);
    camera_controller_->control(camera_.get());
}

void World::bind_player_controller(IPlayerController* controller) {
    player_controller = controller;
}

EventResult World::on_player_action_committed(const PlayerActionCommitted& e) {
    if (e.action == nullptr) [[unlikely]] {
        return EventResult::Continue;
    }
    Profiler::timer("Update").start();

    events_->engine->trigger<WorldUpdatedPreEvent>();

    // give all entities in the world energy according to the cost of the action
    const auto cost = e.action->cost();
    for (auto& entity: entity_reader_->entities()) {
        entity->update(cost);
    }

    // process actions in order
    action_processor_->process_actions();

    // inform entities that actions have been processed
    for (auto& entity: entity_reader_->entities()) {
        entity->on_after_actions();
    }

    events_->engine->trigger<WorldUpdatedPostEvent>();
    Profiler::timer("Update").stop();

    return EventResult::Continue;
}

const Camera& World::get_camera() const {
    return *camera_;
}

CameraController& World::get_camera_controller() {
    return *camera_controller_;
}
