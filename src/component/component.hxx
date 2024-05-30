#ifndef JARALYN_COMPONENT_HXX
#define JARALYN_COMPONENT_HXX

enum class ComponentType {
    Unknown = 0,
    Skills = 1,
    Behavior = 2,
    Render = 3,
    Vision = 4,
};

class Entity;

/**
 * @brief Component interface.
 *
 * Derive from GenericComponent instead when creating new Component classes.
 */
class Component {
public:
    virtual ~Component() = default;

    /**
     * @brief Returns the type of this component
     */
    ComponentType type() const;

    /**
     * @brief Heap-allocates a new component instance and returns it as a unique_ptr.
     *
     * You are required to manage ownership past the return.
     */
    [[nodiscard]] virtual std::unique_ptr<Component> clone() const = 0;

    /**
     * @brief Implementation defined update cycle for this component.
     *
     * @param dt Advanced time steps due to player action
     */
    virtual void update([[maybe_unused]] u64 dt);

    /**
     * @brief Sets the entity this component belongs to
     */
    void set_owner(Entity* entity);

    /**
     * @brief Called after all actions have been performed in the current turn
     */
    virtual void on_after_actions();

    /**
     * @brief Called when the player assumes control of the owning entity
     */
    virtual void on_player_attached();

    /**
     * @brief Called when the player relinquishes control of the owning entity
     */
    virtual void on_player_detached();
protected:
    /**
     * @brief Derive components from GenericComponent instead.
     */
    explicit Component(ComponentType type);

    /**
     * @brief Runtime type of this component used to downcast Component pointers
     */
    ComponentType type_ = ComponentType::Unknown;

    /**
     * @brief Entity this component belongs to
     */
    Entity* entity_ = nullptr;

    /**
     * @brief Called when the owning entity of this component is updated
     */
    virtual void on_owner_updated();
};

template<typename T>
concept ComponentDerived = std::derived_from<T, Component>;

#endif
