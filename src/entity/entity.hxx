#ifndef JARALYN_ACTOR_HXX
#define JARALYN_ACTOR_HXX

#include "components/component.hxx"

struct Entity {
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
     * Returns nullptr if the no component of that type currently exists.
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

    template<typename iter>
    void copy_components(iter it, iter end) {
        while (it != end) {
            const auto& cloneable_component = *it;
            ++it;
            const auto& by_type_it = components_by_type_.find(cloneable_component->type());
            if (by_type_it != components_by_type_.cend()) {
                Log::error("Duplicate component type {} in entity {}", cloneable_component->type(), id);
                continue;
            }
            std::unique_ptr<Component> component = cloneable_component->clone();
            component->set_entity_id(id);
            components_.push_back(std::move(component));
        }
        reindex_components();
    }

    void update(u64 dt);

    [[nodiscard]] bool component_exists(ComponentType type) const;
private:
    std::vector<std::unique_ptr<Component>> components_;
    std::unordered_map<ComponentType, Component*> components_by_type_;

    /**
     * @brief Rebuilds dictionaries and caches of the component store.
     *
     * Should be called every time the component store changes in structure which
     * might cause pointers in the dictionaries to become invalidated.
     */
    void reindex_components();
};

#endif
