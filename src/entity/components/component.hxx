#ifndef JARALYN_COMPONENT_HXX
#define JARALYN_COMPONENT_HXX

enum class ComponentType {
    Unknown = 0,
    Skills = 1,
    Behavior = 2,
    Render = 3,
};

/**
 * @brief Component interface.
 *
 * Derive from GenericComponent instead when creating new Component classes.
 */
class Component {
    friend class EntityFactory;
public:
    virtual ~Component() = default;

    /**
     * @brief Returns the type of this component
     */
    [[nodiscard]] ComponentType type() const;

    /**
     * @brief Returns the entity id this component belongs to
     */
    [[nodiscard]] Id entity_id() const;

    /**
     * @brief Heap-allocates a new component instance and returns it as a unique_ptr.
     *
     * You are required to manage ownership past the return.
     */
    [[nodiscard]] virtual std::unique_ptr<Component> clone() const = 0;

    /**
     * @brief Assigns the entity id
     */
    virtual void set_entity_id(Id entity_id);

    /**
     * @brief Implementation defined update cycle for this component.
     *
     * @param dt Advanced time steps due to player action
     */
    virtual void update(u64 dt);
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
     * @brief Entity id this component belongs to
     */
    Id entity_id_ = null_id;
};

template<typename T>
concept ComponentDerived = std::derived_from<T, Component>;

#endif
