#ifndef JARALYN_ACTION_CREATOR_HXX
#define JARALYN_ACTION_CREATOR_HXX

class Entity;
class Action;
enum class ActionType;

// defined further below
enum class CreateActionError;
struct CreateActionResult;


class IActionCreator {
public:
    /**
     * @brief Factory method to create an action for the given entity.
     * 
     * Injects all necessary dependencies into the action and sets the actor.
     * The action's cost is also calculated here and depends on the speed of the
     * entity. Later changes to the Entity's speed will will not affect this
     * cost.
     * 
     * Creation of the action may fail due to various reasons in which case 
     * the result will contain an error and no action.
     * 
     * @param type Type of the action to create.
     * @param actor Entity that will perform the action.
     * @param player_action Whether the action is performed by the player (TODO: refactor this out later)
     * 
     * @return CreateActionResult Result of the creation.
     */
    virtual CreateActionResult create_action(ActionType type, Entity& actor, bool player_action = false) = 0;

    virtual ~IActionCreator() = default;
};

enum class CreateActionError {
    /**
     * @brief Action was created successfully.
     */
    None,

    /**
     * @brief The action type is invalid.
     */
    InvalidType,

    /**
     * @brief The entity does not have enough energy to perform the action.
     */
    NoEnergy,
};

struct CreateActionResult {
    /**
     * @brief Error that occurred during the creation of the action.
     */
    CreateActionError error;

    /**
     * @brief Created action or null if error is set
     */
    Action* action;

    /**
     * @brief Checks if the creation of the action failed.
     */
    bool failed() const {
        return error != CreateActionError::None;
    }
};

#endif
