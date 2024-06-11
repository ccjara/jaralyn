#ifndef NOEDEN_ENTITY_MANAGER_HXX
#define NOEDEN_ENTITY_MANAGER_HXX

#include "entity/entity_reader.hxx"
#include "entity/entity_writer.hxx"

class EntityManager : public IEntityReader, public IEntityWriter {
public:
    explicit EntityManager(Events* events);

    /**
     * @copydoc IEntityReader::entity
     */
    Entity* entity(Id id) override;

    /**
     * @copydoc IEntityReader::entity
     */
    const Entity* entity(Id id) const override;

    /**
     * @copydoc IEntityReader::entities
     */
    EntityContainer& entities() override;

    /**
     * @copydoc IEntityReader::entities
     */
    const EntityContainer& entities() const override;

    /**
     * @copydoc IEntityReader::player
     */
    Entity* player() override;

    /**
     * @copydoc IEntityReader::player
     */
    const Entity* player() const override;

    /**
     * @copydoc IEntityWriter::create_entity
     */
    Entity& create_entity(const Archetype& archetype, const WorldPos& position) override;

    /**
     * @copydoc IEntityWriter::set_controlled_entity
     */
    ControlEntityResult set_controlled_entity(Entity* entity) override;

    /**
     * @copydoc IEntityWriter::set_controlled_entity
     */
    ControlEntityResult set_controlled_entity(Id id) override;
private:
    /**
     * @brief Primary (owning) container of all entities
     */
    EntityContainer entities_;

    /**
     * @brief Indexes entities by their ID.
     * 
     * The value (index) points to the entity's position in the entities_ vector.
     */
    std::unordered_map<Id, size_t> index_by_id_;

    /**
     * @brief Entity currently controlled by the player
     */
    Entity* controlled_entity_ = nullptr;

    Events* events_ = nullptr;
};

#endif
