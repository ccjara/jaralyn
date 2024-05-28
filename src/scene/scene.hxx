#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../entity/action.hxx"
#include "../events/event_manager.hxx"
#include "../grid.hxx"
#include "../input/input_event.hxx"
#include "../entity/fov.hxx"

struct Archetype;

/**
 * @brief Contains and manages entities.
 */
class Scene {
    using EntityContainer = std::vector<std::unique_ptr<Entity>>;
public:
    static void init(EventManager* events);
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
     * @brief Updates the scene, performing all actions in the action queue.

     * The queue is sorted before it is processed, faster actions will get
     * processed first. Speed is calculated upon pushing an action onto the
     * queue and is unaffected by speed changes of other actions.
     */
    static void update();

    /**
     * @brief Draws the scene
     */
    static void draw();

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

    /**
     * @brief Applies the FoV of an Entity to the tile grid.
     */
    static void apply_fov(Vec2<i32> entity_pos, const Fov& fov);

    static Id player_id();
private:
    static bool on_key_down(KeyDownEvent& e);

    static inline EntityContainer entities_;
    static inline Id player_id_ = null_id;
    static inline Action* player_action_ = nullptr;
    static inline std::unordered_map<Id, size_t> entity_id_to_index_;
    static inline std::unordered_map<ComponentType, std::vector<Entity*>> entities_by_components_;

    static inline Grid<Tile> tiles_;

    static inline EventManager* events_ = nullptr;
};

#endif
