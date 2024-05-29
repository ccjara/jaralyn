#include "move_action.hxx"
#include "components/skills.hxx"
#include "scene/tile.hxx"
#include "entity/entity.hxx"
#include "world/tile_reader.hxx"

MoveAction::MoveAction(IEntityReader* entity_reader, ITileReader* tile_reader) :
    entity_reader_(entity_reader),
    tile_reader_(tile_reader) {
    assert(entity_reader_);
    assert(tile_reader_);
    type_ = ActionType::Move;
}

ActionResult MoveAction::perform() {
    if (!entity_) {
        return ActionResult::Failure;
    }
    const Tile* dest_tile = tile_reader_->tile(destination);

    if (dest_tile == nullptr) {
        return ActionResult::Failure;
    }

    if (dest_tile->solid) {
        return ActionResult::Failure;
    }

    if (dest_tile->type == TileType::Water) {
        Skills* skills = entity_->component<Skills>();
        if (skills) {
            skills->increase_progress(SkillId::Swim, 1);
        }
    }

    entity_->position = destination;

    return ActionResult::Failure;
}
