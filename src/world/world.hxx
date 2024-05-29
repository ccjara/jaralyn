#ifndef JARALYN_WORLD_HXX
#define JARALYN_WORLD_HXX

#include "entity_reader.hxx"
#include "entity_writer.hxx"
#include "tile_reader.hxx"

class IPlayerController;

class World : public IEntityReader, public IEntityWriter, public ITileReader {
public:
    /**
     * @copydoc IEntityReader::entity
     */
    Entity* entity(Id id) override;

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
    Entity& create_entity(const Archetype& archetype) override;

    /**
     * @copydoc ITileReader::tile
     */
    Tile* tile(Vec2<i32> position) const override;

    /**
     * @copydoc ITileReader::tiles
     */
    Grid<Tile> const& tiles() const override;

    /**
     * @copydoc ITileReader::tiles
     */
    Grid<Tile>& tiles() override;
private:
    EntityContainer entities_;

    /**
     * @brief Indexes entities by their ID.
     * 
     * The value (index) points to the entity's position in the entities_ vector.
     */
    std::unordered_map<Id, size_t> index_by_id;

    /**
     * @brief Player controller bound to this world
     */
    IPlayerController* player_controller = nullptr;
};

#endif
