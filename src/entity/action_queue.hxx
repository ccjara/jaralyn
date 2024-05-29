#ifndef JARALYN_ACTION_QUEUE_HXX
#define JARALYN_ACTION_QUEUE_HXX

#include "action_creator.hxx"
#include "action_processor.hxx"
#include "../events/event_manager.hxx"

class EntitiesUpdated;

class ActionQueue : public IActionCreator, public IActionProcessor {
public:
    explicit ActionQueue(EventManager* event_manager, ServiceLocator* services);

    /**
     * @copydoc IActionCreator::create_action
    */
    CreateActionResult create_action(ActionType type, Entity& actor, bool player_action = false) override;

    /**
     * @copydoc IActionProcessor::process_actions
    */
    void process_actions() override;
private:
    std::unique_ptr<Action> instantiate_action(ActionType type);

    bool on_entities_updated(const EntitiesUpdated& event);

    bool sufficient_energy(ActionType type, Entity& actor) const;

    u32 base_cost(ActionType type) const;

    std::vector<std::unique_ptr<Action>> actions_;

    EventManager* events_ = nullptr;
    ServiceLocator* services_ = nullptr;
};

#endif