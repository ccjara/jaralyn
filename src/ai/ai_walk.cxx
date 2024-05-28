#include "ai_walk.hxx"
#include "../entity/entity.hxx"
#include "../entity/action_creator.hxx"
#include "../entity/move_action.hxx"
#include "../world/entity_provider.hxx"

AiWalk::AiWalk(IActionCreator* action_creator, IEntityProvider* entity_provider) : 
    action_creator_(action_creator), 
    entity_provider_(entity_provider) {
    assert(action_creator_);
    assert(entity_provider_);
}

void AiWalk::clear() {
    mod_state(AiNodeState::Ready);
    target_type_ = WalkTargetType::Random;
    entity_target_key_ = default_target_key;
}

AiNodeState AiWalk::visit(AiContext& context) {
    static int dir = 0;
    auto entity = entity_provider_->get_entity_by_id(context.entity_id);
    if (!entity) {
        return mod_state(AiNodeState::Failed);
    }

    if (entity->energy < MoveAction::BASE_COST / entity->speed) {
        return mod_state(AiNodeState::Failed);
    }

    if (target_type_ == WalkTargetType::Entity) {
        return perform_walk_to_entity(context);
    } else {
        return perform_walk_around(context);
    }
}

AiNodeState AiWalk::perform_walk_to_entity(AiContext& context) {
    assert(context.entity);
    auto target_id = context.blackboard.get<Id>(entity_target_key_);
    if (!target_id) {
        return mod_state(AiNodeState::Failed);
    }
    auto target = entity_provider_->get_entity_by_id(*target_id);
    if (!target) {
        return mod_state(AiNodeState::Failed);
    }

    auto position = context.entity->position;
    const auto target_position = target->position;

    if (position.x < target_position.x) {
        ++position.x;
    } else if (position.x > target_position.x) {
        --position.x;
    }
    if (position.y < target_position.y) {
        ++position.y;
    } else if (position.y > target_position.y) {
        --position.y;
    }

    action_creator_->create_action(*context.entity, std::make_unique<MoveAction>(position));

    return mod_state(AiNodeState::Failed); // TODO: this must be changed later when repeatable actions (by conditions) are implemented
}

AiNodeState AiWalk::perform_walk_around(AiContext& context) {
    auto pos = context.entity->position;

    static int dir = 0;
    if (dir == 0) {
        --pos.y;
    } else if (dir == 1) {
        ++pos.x;
    } else if (dir == 2) {
        ++pos.y;
    } else if (dir == 3) {
        --pos.x;
    }
    ++dir %= 4;

    action_creator_->create_action(*context.entity, std::make_unique<MoveAction>(pos));
    return mod_state(AiNodeState::Succeeded);
}

void AiWalk::target_entity(const std::string& entity_target_key) {
    target_type_ = WalkTargetType::Entity;
    entity_target_key_ = entity_target_key;
}

void AiWalk::walk_around() {
    target_type_ = WalkTargetType::Random;
    entity_target_key_ = default_target_key;
}