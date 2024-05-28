#ifndef JARALYN_ACTION_CREATOR_HXX
#define JARALYN_ACTION_CREATOR_HXX

class Entity;
class Action;

class IActionCreator {
public:
    /**
     * @brief Creates an action for the given entity.
     *
     * The action's cost is calculated here and depends on the speed of the
     * entity. Later changes to the Entity's speed will will not affect this
     * cost.
     */
    virtual void create_action(Entity& actor, std::unique_ptr<Action>&& action) = 0;

    virtual ~IActionCreator() = default;
};

#endif
