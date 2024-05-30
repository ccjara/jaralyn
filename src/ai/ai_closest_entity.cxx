#include "ai/ai_closest_entity.hxx"
#include "action/action.hxx"
#include "entity/entity.hxx"
#include "component/vision/vision.hxx"
#include "entity/entity_reader.hxx"

AiClosestEntity::AiClosestEntity(IEntityReader* entity_reader) : entity_reader_(entity_reader) {
    assert(entity_reader_);
}

AiNodeState AiClosestEntity::visit(AiContext& context) {
    if (!context.entity) {
        return mod_state(AiNodeState::Failed);
    }

    // OPTIMIZE: could subscribe to a `VisionRadiusUpdated` event to then
    //           cache the radius in the blackboard

    Vision* vision = context.entity->component<Vision>();
    if (vision == nullptr) {
        return mod_state(AiNodeState::Failed);
    }
    const auto radius = vision->vision_radius();

    // OPTIMIZE: quadtree
    for (const auto& entity : entity_reader_->entities()) {
        if (entity->id == context.entity_id) {
            continue;
        }
        if (entity->position.manhattan_distance(context.entity->position) > radius) {
            continue;
        }
        context.blackboard.set(found_target_key, entity->id);
        return mod_state(AiNodeState::Succeeded);
    }

    // no unit in range matching the filter found
    return mod_state(AiNodeState::Failed);
}

void AiClosestEntity::clear() {
    mod_state(AiNodeState::Ready);
}

void AiClosestEntity::set_found_target_key(std::string&& key) {
    found_target_key = std::move(key);
}

void AiClosestEntity::set_found_target_key(const std::string& key) {
    found_target_key = key;
}
