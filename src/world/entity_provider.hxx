#ifndef JARALYN_ENTITY_PROVIDER_HXX
#define JARALYN_ENTITY_PROVIDER_HXX

class Entity;

/**
 * @brief Provides entity access
 */
class IEntityProvider {
public:
    /**
     * @brief Gets an entity by id
     * 
     * @param id ID of the entity to find
     * 
     * @return Entity* Target entity or nullptr if not found
     */
    virtual Entity* get_entity_by_id(Id id) = 0;

    virtual ~IEntityProvider() = default;
};

#endif
