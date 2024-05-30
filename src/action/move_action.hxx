#ifndef JARALYN_MOVE_ACTION_HXX
#define JARALYN_MOVE_ACTION_HXX

#include "action/action.hxx"

class IEntityReader;
class ITileReader;

/**
 * @brief Action representing the movement of an entity.
 *
 * Action may fail due to blockages, being movement impaired, etc.
 */
class MoveAction : public Action {
public:
    explicit MoveAction(IEntityReader* entity_reader, ITileReader* tile_reader);

    ActionResult perform() override;

    /**
     * @brief Destination this entity will arrive at if the action succeeds.
     */
    Vec2<i32> destination;
private:
    IEntityReader* entity_reader_ = nullptr;
    ITileReader* tile_reader_ = nullptr;
};

#endif
