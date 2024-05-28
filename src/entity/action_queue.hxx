#ifndef JARALYN_ACTION_QUEUE_HXX
#define JARALYN_ACTION_QUEUE_HXX

#include "action_creator.hxx"
#include "action_processor.hxx"
#include "../events/event_manager.hxx"

class EntitiesUpdated;

class ActionQueue : public IActionCreator, public IActionProcessor {
public:
    explicit ActionQueue(EventManager* event_manager);

    /**
     * @copydoc IActionCreator::create_action
    */
    void create_action(Entity& actor, std::unique_ptr<Action>&& action) override;

    /**
     * @copydoc IActionProcessor::process_actions
    */
    void process_actions() override;
private:
    bool on_entities_updated(const EntitiesUpdated& event);

    std::vector<std::unique_ptr<Action>> actions_;
    EventManager* events_;
};

#endif