#ifndef JARALYN_ACTION_HXX
#define JARALYN_ACTION_HXX

class Entity;

/**
 * @brief Base class for an action any controller may produce for an Entity.
 *
 * TODO: Rename to Command?
 */
struct Action {
    virtual ~Action() = default;

    /**
     * @brief Extension point for subclasses defining a concrete action.
     *
     * Returns true if the action was performed successfully.
     */
    virtual bool perform() = 0;

    /**
     * @brief Must be specified for each concrete action to calculate the cost.
     */
    virtual u32 base_cost() const = 0;

    /**
     * @brief Points to the Entity performing this action.
     */
    Entity* entity { nullptr };

    /**
     * @brief Speed at which this action performs.
     *
     * This speed is unaffected by changes to the entity's speed being made
     * after this action was created.
     */
    u32 speed { 0U };

    /**
     * @brief Calculated cost based on base cost and speed.
     */
    u32 cost { 0U };
};

#endif
