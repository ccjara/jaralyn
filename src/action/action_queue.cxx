#include "action/action_queue.hxx"
#include "action/action.hxx"
#include "action/action_event.hxx"
#include "action/move_action.hxx"
#include "action/null_action.hxx"
#include "entity/entity_event.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"
#include "tile/tile_reader.hxx"
#include "tile/tile.hxx"

ActionQueue::ActionQueue(EventManager* event_manager, ServiceLocator* services) : 
    events_(event_manager),
    services_(services) {
    assert(events_);
    assert(services_);
}

CreateActionResult ActionQueue::create_action(ActionType type, Entity& actor, bool player_action) {
    if (!player_action && !sufficient_energy(type, actor)) {
        return { CreateActionError::NoEnergy, nullptr };
    }

    std::unique_ptr<Action> action = instantiate_action(type);
    action->entity_ = &actor;
    action->speed_ = actor.speed;

    if (action->speed_ < 1) {
        action->speed_ = 1;
    }

    action->cost_ = base_cost(type) / actor.speed;
 
    if (player_action) {
        actor.energy = 0;
    } else {
        actor.energy -= action->cost_;
    }

    Action* action_raw_ptr = actions_.emplace_back(std::move(action)).get();

    if (player_action) {
        // TODO: temporary code
        services_->get<ITileReader>()->tiles().each([](Tile& tile) { tile.fov = false; });
        auto entity_manager_ = services_->get<IEntityReader>();

        for (auto& entity : entity_manager_->entities()) {
            entity->update(action_raw_ptr->cost());
        }

        for (auto& entity : entity_manager_->entities()) {
            entity->on_after_actions();
        }
    }

    return { CreateActionError::None, action_raw_ptr };
}

std::unique_ptr<Action> ActionQueue::instantiate_action(ActionType type) {
    switch (type) {
        case ActionType::None:
            return std::make_unique<NullAction>();
        case ActionType::Move:
            return std::make_unique<MoveAction>(services_->get<IEntityReader>(), services_->get<ITileReader>());
        default:
            return std::unique_ptr<Action>();
    }
}

void ActionQueue::process_actions() {
    std::sort(
        actions_.begin(),
        actions_.end(),
        [](const auto& a, const auto& b) {
            return a->speed_ > b->speed_;
        }
    );
    for (auto&& action : actions_) {
        action->perform();
    }
    actions_.clear();

    events_->trigger<ActionQueueProcessed>();
}

bool ActionQueue::sufficient_energy(ActionType type, Entity& actor) const {
    return base_cost(type) / actor.speed <= actor.energy;
}

u32 ActionQueue::base_cost(ActionType type) const {
    // TODO: scriptable / configurable later
    switch (type) {
        case ActionType::None:
        default:
            return 0;
        case ActionType::Move:
            return 100;
    }
}
