#ifndef JARALYN_WORLD_HXX
#define JARALYN_WORLD_HXX

#include "entity_provider.hxx"

class Entity;

class World : public IEntityProvider {
public:
    /**
     * @copydoc IEntityProvider::get_entity_by_id
     */
    Entity* get_entity_by_id(Id id) override;
};

#endif
