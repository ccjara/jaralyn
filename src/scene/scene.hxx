#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../entity/entity.hxx"
#include "../entity/move_action.hxx"
#include "../entity/entity_factory.hxx"
#include "../events/game_event.hxx"
#include "../grid.hxx"
#include "tile_builder.hxx"
#include "../input/input_event.hxx"
#include "fov.hxx"

/**
 * @brief Contains and manages entities.
 */
class Scene {
    using ActionContainer = std::vector<std::unique_ptr<Action>>;
    using EntityContainer = std::vector<std::unique_ptr<Entity>>;
public:
    static void init();
    static void shutdown();

    /**
     * @brief Returns a pointer to an Entity for the given id or nullptr if not found.
     */
    static Entity* get_entity_by_id(Id id);

    /**
     * @brief Constructs an Entity of the given archetype and returns it.
     *
     * The Entity can be further configured after creation.
     */
    static Entity& create_entity(const Archetype& archetype);

    /**
     * @brief Provides write access to the Entity container.
     */
    static EntityContainer& entities();

    /**
     * @brief Provides readonly access to the Entity container.
     */
    static const EntityContainer& read_entities();

    /**
     * @brief Provides write access to the tile container.
     */
    static Grid<Tile>& tiles();

    /**
     * @brief Provides readonly access to the tile grid.
     */
    static const Grid<Tile>& read_tiles();

    /**
     * @brief Writes the field of view to the display
     */
    static void update_fov(u64 entity);

    /**
     * @brief Executes each action in order of the Actions' calculated speed.
     *
     * The queue is sorted before it is processed, faster actions will get
     * processed first. Speed is calculated upon pushing an action onto the
     * queue and is unaffected by speed changes of other actions.
     */
    static void perform_actions();

    /**
     * @brief Constructs an action in place inside the action queue.
     *
     * The action's cost is calculated here and depends on the speed of the
     * entity. Later changes to the Entity's speed will will not affect this
     * cost.
     *
     * Returns a reference to the created action.
     */
    template<typename A, typename... EntityArgs>
    static Action& create_action(Entity* entity, EntityArgs&&... args) {
        assert(entity);

        auto& action = *actions_.emplace_back(
            new A(std::forward<EntityArgs>(args)...)
        ).get();

        action.entity = entity;
        action.cost = action.speed * action.base_cost();
        return action;
    }

    /**
     * @brief Assigns the given Entity id as the new player to control.
     *
     * May be and is initially unset to signify no player currently exists.
     */
    static void set_player(Id id);

    /**
     * @brief Returns the current Entity regarded as a player if any.
     */
    static Entity* player();

    static Id player_id();
private:
    static bool on_key_down(KeyDownEvent& e);

    static inline EntityContainer entities_;
    static inline ActionContainer actions_;
    static inline Id player_id_ = null_id;
    static inline Action* player_action_ = nullptr;
    static inline std::unordered_map<Id, size_t> entity_id_to_index_;

    static inline Grid<Tile> tiles_;
};

#endif
