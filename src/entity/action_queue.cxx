#include "action_queue.hxx"
#include "action_event.hxx"
#include "entity_event.hxx"
#include "./action.hxx"
#include "./entity.hxx"

ActionQueue::ActionQueue(EventManager* event_manager) : events_(event_manager) {
    assert(events_);

    events_->on<EntitiesUpdated>(this, &ActionQueue::on_entities_updated);
}

bool ActionQueue::on_entities_updated(const EntitiesUpdated& event) {
    process_actions();
    return false;
}

void ActionQueue::create_action(Entity& actor, std::unique_ptr<Action>&& action) {
    action->entity = &actor;
    action->speed = actor.speed;

    if (action->speed < 1) {
        action->speed = 1;
    }

    action->cost = action->base_cost() / actor.speed;
 
    actor.energy -= action->cost;

    actions_.emplace_back(std::move(action));
}

void ActionQueue::process_actions() {
    std::sort(
        actions_.begin(),
        actions_.end(),
        [](const auto& a, const auto& b) {
            return a->speed > b->speed;
        }
    );
    for (auto&& action : actions_) {
        action->perform();
    }
    actions_.clear();

    events_->trigger<ActionQueueProcessed>();
}