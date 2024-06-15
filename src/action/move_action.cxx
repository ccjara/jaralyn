#include "action/move_action.hxx"
#include "component/skills.hxx"
#include "entity/entity.hxx"
#include "entity/entity_event.hxx"
#include "tile/tile.hxx"
#include "world/tile_accessor.hxx"

MoveAction::MoveAction(IEntityReader* entity_reader, TileAccessor* tile_accessor, Events* events) :
    entity_reader_(entity_reader),
    tile_accessor_(tile_accessor),
    events_(events) {
    assert(entity_reader_);
    assert(tile_accessor_);
    type_ = ActionType::Move;
}

ActionResult MoveAction::perform() {
    if (!entity_) {
        return ActionResult::Failure;
    }
    const Tile* current_tile = tile_accessor_->get_tile(entity_->position);
    const Tile* dest_tile = tile_accessor_->get_tile(destination);

    if (dest_tile == nullptr || dest_tile->type == TileType::Empty) {
        return ActionResult::Failure;
    }

    if (dest_tile->type != TileType::Floor) {
        if (!current_tile || !current_tile->flags.test(TileFlags::Ramp)) {
            return ActionResult::Failure;
        }

        // unit is standing on a ramp and moving into a blocking tile.
        // only allow moving up if there is a corresponding down ramp one tile up
        const auto elevated_position = entity_->position + WorldPos(0, 1, 0);
        const Tile* elevated_tile = tile_accessor_->get_tile(elevated_position);
        if (!elevated_tile || !elevated_tile->flags.test(TileFlags::Ramp)) {
            return ActionResult::Failure;
        }

        // test whether the move into that direction would be blocked (for example when standing at the ramp and moving diagonally but after elevation and then
        // moving diagonally there would be a wall)
        const auto elevated_dest = destination + WorldPos(0, 1, 0);
        const Tile* elevated_dest_tile = tile_accessor_->get_tile(elevated_dest);
        if (!elevated_dest_tile || elevated_dest_tile->flags.test(TileFlags::Blocking)) {
            return ActionResult::Failure;
        }
        ++destination.y;
    } else if (dest_tile->flags.test(TileFlags::Ramp)) {
        // unit is moving onto a ramp
        // if there is a down ramp below, teleport them down
        const Tile* tile_below = tile_accessor_->get_tile(destination - WorldPos(0, 1, 0));
        if (tile_below && tile_below->flags.test(TileFlags::Ramp)) {
            --destination.y;
        }
    }

    if (dest_tile->material == MaterialType::Water) {
        Skills* skills = entity_->component<Skills>();
        if (skills) {
            skills->increase_progress(SkillId::Swim, 1);
        }
    }

    entity_->position = destination;

    if (entity_->player_attached_) {
        events_->engine->trigger<PlayerMovedEvent>(entity_->position);
    }

    return ActionResult::Success;
}
