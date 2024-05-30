#ifndef JARALYN_ENTITY_READER_HXX
#define JARALYN_ENTITY_READER_HXX

class Entity;

/**
 * @brief Provides entity access
 */
class IEntityReader {
public:
    using EntityContainer = std::vector<std::unique_ptr<Entity>>;

    /**
     * @brief Gets a mutable entity by id
     * 
     * @param id ID of the entity to find
     * 
     * @return Entity* Target entity or nullptr if not found
     */
    virtual Entity* entity(Id id) = 0;

    /**
     * @brief Gets an immuatble entity by id
     * 
     * @param id ID of the entity to find
     * 
     * @return Entity* Target entity or nullptr if not found
     */
    virtual const Entity* entity(Id id) const = 0;

    /**
     * @brief Provides write access to the entity container
     * 
     * @return EntityContainer& Readable entity container
     */
    virtual EntityContainer& entities() = 0;

    /**
     * @brief Gets the player entity
     * 
     * @return Entity* Player entity or nullptr if not found
     */
    virtual Entity* player() = 0;

    /**
     * @brief Gets the player entity
     * 
     * @return const Entity* Player entity or nullptr if not found
     */
    virtual const Entity* player() const = 0;

    /**
     * @brief Provides read access to the entity container
     * 
     * @return const EntityContainer& Writable entity container
     */
    virtual const EntityContainer& entities() const = 0;

    virtual ~IEntityReader() = default;
};

#endif
