#ifndef NOEDEN_ENTITY_WRITER_HXX
#define NOEDEN_ENTITY_WRITER_HXX

class Entity;
struct Archetype;

enum class ControlEntityResult {
    Success,
    EntityAlreadyControlled,
    EntityNotFound,
    EntityNotControllable,
};

class IEntityWriter {
public:
    /**
     * @brief Creates an Entity of the given archetype and returns it.
     *
     * The Entity can be further configured after creation.
     */
    virtual Entity& create_entity(const Archetype& archetype, const WorldPos& position) = 0;

    /**
     * @brief Set the player controlled entity
     * 
     * @param id Entity entity to control
     */
    virtual ControlEntityResult set_controlled_entity(Entity* entity) = 0;

    /**
     * @brief Set the player controlled entity.
     * 
     * Resolves the entity and then calls set_controlled_entity(Entity* entity)
     * 
     * @param id ID of entity to control
     */
    virtual ControlEntityResult set_controlled_entity(Id id) = 0;

    virtual ~IEntityWriter() = default;
};

#endif
