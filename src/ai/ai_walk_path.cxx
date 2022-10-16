#include "ai_walk_path.hxx"

void AiWalkPath::clear() {
}

AiNodeState AiWalkPath::visit(AiContext& context) {
    return mod_state(AiNodeState::Succeeded);
}
