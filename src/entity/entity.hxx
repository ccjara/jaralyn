#ifndef JARALYN_ACTOR_HXX
#define JARALYN_ACTOR_HXX

#include "component/component.hxx"

class Entity {
public:
    friend class EntityFactory;

    Entity();

    Id id;
    static inline Id next_id_ = 1U;

    std::string name = "NYI"; // TODO

    Vec2<i32> position;

    i32 speed { 0 };
    i32 energy { 0 };

    /**
     * @brief Locates a component by the given template argument.
     *
     * Returns nullptr if no component of that type currently exists.
     */
    template<ComponentDerived Comp>
    Comp* component() const {
        static_assert(Comp::static_type() != ComponentType::Unknown);
        auto iter = components_by_type_.find(Comp::static_type());
        if (iter == components_by_type_.end()) {
            return nullptr;
        }
        return static_cast<Comp*>(iter->second);
    }

    /**
     * @brief Returns true if the entity has a component of the given type
     */
    inline bool has_component(ComponentType type) const;

    /**
     * @brief Takes ownership of the given component pointer and inserts it into the component store.
     *
     * If a component of that type already exists, the given component will be deleted and not inserted.
     * 
     * Returns true if the component was successfully added.
     */
    bool add_component(std::unique_ptr<Component>&& component);

    void update(u64 dt);

    /**
     * @brief Called after all actions have been processed in the current turn.
     * 
     * @todo: This will likely become a performance problem later and will likely require
     *        usage of the event manager, but it still lacks support to unsubscribe from events.
     */
    void on_after_actions();

    /**
     * @brief Called after the player assumed control of this entity.
     */
    void on_player_attached();

    /**
     * @brief Called after the player relinquishes control of this entity.
     */
    void on_player_detached();
private:
    std::vector<std::unique_ptr<Component>> components_;
    std::unordered_map<ComponentType, Component*> components_by_type_;
};

#endif
