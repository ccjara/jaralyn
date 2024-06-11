#ifndef NOEDEN_MOVE_ACTION_HXX
#define NOEDEN_MOVE_ACTION_HXX

#include "action/action.hxx"

class IEntityReader;
class TileAccessor;

/**
 * @brief Action representing the movement of an entity.
 *
 * Action may fail due to blockages, being movement impaired, etc.
 */
class MoveAction : public Action {
public:
    explicit MoveAction(IEntityReader* entity_reader, TileAccessor* tile_accessor);

    ActionResult perform() override;

    /**
     * @brief Destination this entity will arrive at if the action succeeds.
     */
    WorldPos destination;
private:
    IEntityReader* entity_reader_ = nullptr;
    TileAccessor* tile_accessor_ = nullptr;
};

#endif
