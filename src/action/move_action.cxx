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
    const Tile* dest_tile = tile_accessor_->get_tile(destination);

    if (dest_tile == nullptr) {
        return ActionResult::Failure;
    }

    if (dest_tile->flags.test(TileFlags::Blocking)) {
        return ActionResult::Failure;
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
